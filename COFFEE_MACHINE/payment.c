/*
 * payment.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "payment.h"
/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
INT8U cardnumber[CARD_LENGTH];
extern QueueHandle_t ds_input_queue;

INT8U balance;
SemaphoreHandle_t balance_mutex;

QueueSetHandle_t cash_set;

extern TaskHandle_t coffee_t;

extern SemaphoreHandle_t active_semaphore;
/*****************************   Functions   *******************************/
/*****************************************************************************
 *   Input    : N/A
 *   Output   : N/A
 *   Function : Initialize payment
 ******************************************************************************/

void payment_init()
{
    balance_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(balance_mutex);

    // Set cotaining digiswtich input queue and coffee select semaphore
    cash_set = xQueueCreateSet(DS_INPUT_QUEUE_LENGTH + 1);
}

/*****************************************************************************
 *   Input    : pvParameters
 *   Output   : N/A
 *   Function : Payment state machine
 ******************************************************************************/
void payment_task(void* pvParameters)
{
    PAYMENT_STATES current_state = START;

    configASSERT(xQueueAddToSet(ds_input_queue, cash_set) == pdPASS);
    configASSERT(xQueueAddToSet(active_semaphore, cash_set) == pdPASS);
    xSemaphoreGive(active_semaphore);

    while (1)
    {

        switch (current_state)
        {
        case START:
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            current_state = PAYMENT;
            break;
        case PAYMENT:
            current_state = paymenttype_state();
            break;
//        case CARD:
//            current_state = CARD_NUMBER;
            break;
        case CARD_NUMBER:
            current_state = cardnumber_check_state();
            break;
        case PIN:
            current_state = pin_check_state();
            break;
        case CASH:
            current_state = cash_state();
            break;
        case CHANGE:
            current_state = change_state();
            break;
        case LOG:
            log_payment(cardnumber);
            current_state = START;
            break;
        }
    }
}

/*****************************************************************************
 *   Input    :N/A
 *   Output   :Payment state
 *   Function :Payment type state logic
 ******************************************************************************/
PAYMENT_STATES paymenttype_state()
{
    lprintf(0, "Payment method");
    lprintf(1, "%c Card | %c Cash", CARD_METHOD, CASH_METHOD);
    // Display "card or cash?"

    // Wait for key input getKey(portMAX_DELAY)
    while (1)
    {
        switch (key_get(portMAX_DELAY))
        {
        case CARD_METHOD:
            return CARD_NUMBER;

        case CASH_METHOD:
            return CASH;
        default:
            break;

        }
    }

}

/*****************************************************************************
 *   Input    :N/A
 *   Output   : Payment state
 *   Function :Cardnumber state logic / check for 8 digit card number
 ******************************************************************************/
PAYMENT_STATES cardnumber_check_state()
{
    lprintf(0, "Enter cardnumber");
    lprintf(1, ""); // Reset bottom ready for writing digits

    INT8U digit_counter = 0;

    while (1)
    {
        if (digit_counter < CARD_LENGTH)
        {
            INT8U inp = key_get(portMAX_DELAY);
            if (key2int(inp) != -1)
            {
                cardnumber[digit_counter] = key2int(inp);
                digit_counter++;
                wr_ch_LCD(inp);
            }
        }
        else
        {
            return PIN;
        }
    }
}

/*****************************************************************************
 *   Input    :N/A
 *   Output   :Payment state
 *   Function :Pin state logic / check pin
 ******************************************************************************/
PAYMENT_STATES pin_check_state()
{
    INT8U pin[PIN_LENGTH];
    INT8U pin_counter = 0;
    INT8U attempts = 0;
    while (1)
    {
        if (pin_counter < PIN_LENGTH)
        {
            if (pin_counter == 0)
            {
                lprintf(0, "Enter PIN");
                lprintf(1, ""); // Reset bottom ready for writing digits
            }
            INT8U inp = key_get(portMAX_DELAY);
            if (key2int(inp) != -1)
            {
                pin[pin_counter] = key2int(inp);
                pin_counter++;
                wr_ch_LCD('*');
            }
        }
        else
        {
            if ((pin[PIN_LENGTH - 1] % 2 == 0
                    && cardnumber[CARD_LENGTH - 1] % 2 != 0)
                    || (pin[PIN_LENGTH - 1] % 2 != 0
                            && cardnumber[CARD_LENGTH - 1] % 2 == 0))
            {
                balance = CARD_PREPAID;
                lprintf(1, "");
                xTaskNotifyGive(coffee_t); // Start brewing

                while (1)
                {
                    xQueueSelectFromSet(cash_set, portMAX_DELAY); // Wait for brew

                    if (xSemaphoreTake(active_semaphore, 0) == pdPASS)
                    {
                        xSemaphoreGive(active_semaphore);
                        break;
                    }
                }

                xTaskNotifyGive(coffee_t); // Restart
                return LOG;
            }
            else
            {
                if (attempts < CARD_MAX_ATTEMPTS)
                {
                    lprintf(0, "Incorrect PIN");
                    lprintf(1, "Try again");
                    key_get(portMAX_DELAY);
                    pin_counter = 0;
                    attempts++;
                }
                else
                {
                    return CARD_NUMBER;
                }
            }
        }
    }
}

/*****************************************************************************
 *   Input    : Key in ascii
 *   Output   : Ket in int
 *   Function :Convertion of ascii to int
 ******************************************************************************/
INT8S key2int(INT8U key)
{
    INT8S out = (INT8U) key - '0';
    if (out < 0 || out > 9)
    {
        out = -1;
    }
    return out;
}

/*****************************************************************************
 *   Input    : N/A
 *   Output   : Payment state
 *   Function : Cash state logic / update payment balance
 ******************************************************************************/
PAYMENT_STATES cash_state()
{
    xSemaphoreTake(balance_mutex, portMAX_DELAY);
    balance = 0;
    xSemaphoreGive(balance_mutex);

    lprintf(0, "Insert coins");

    xTaskNotifyGive(coffee_t); // Start brew

    while (1)
    {
        xSemaphoreTake(balance_mutex, portMAX_DELAY);
        INT8S dir = 0;
        do
        {
            dir = digiswitch_get(0);
            switch (dir)
            {
            case 1:
                balance += CASH_CLOCKWISE;
                break;
            case -1:
                balance += CASH_COUNTERCLOCKWISE;
                break;
            }
        }
        while (dir != 0);

        lprintf(1, "Balance: %ikr", balance);

        xSemaphoreGive(balance_mutex);

        xQueueSelectFromSet(cash_set, portMAX_DELAY);

        if (xSemaphoreTake(active_semaphore, 0) == pdPASS)
        {
            xSemaphoreGive(active_semaphore);
            break;
        }
    }

    return CHANGE;
}

/*****************************************************************************
 *   Input    : N/A
 *   Output   : Payment state
 *   Function : Change state / output change / complete transaction
 ******************************************************************************/
PAYMENT_STATES change_state()
{
    int i;
    xSemaphoreTake(balance_mutex, portMAX_DELAY);
    lprintf(0, "Change: %d", balance);
    lprintf(1, "");

    for (i = 0; i < balance; i++)
    {
        led_yellow();
        vTaskDelay(pdMS_TO_TICKS(CHANGE_FLASH_TIME_MS));
        led_off();
        vTaskDelay(pdMS_TO_TICKS(CHANGE_FLASH_TIME_MS));
    }
    xSemaphoreGive(balance_mutex);

    xTaskNotifyGive(coffee_t);

    // Show the logger its a cash payment
    cardnumber[0] = CASH_ID;

    return LOG;
}


