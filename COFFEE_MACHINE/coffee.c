/*
 * coffee.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "coffee.h"
/*****************************    Defines    *******************************/
typedef enum COFFEE_STATES
{
    SELECT_COFFEE, BREW, C_LOG,
} COFFEE_STATES;

/*****************************   Constants   *******************************/
/*****************************   Variables   *******************************/
//enum CoffeeTask_states CoffeeTask_state = Start;
COFFEE_TYPE coffee_types[COFFEE_TYPES_LENGTH];
SemaphoreHandle_t coffee_types_mutex;

COFFEE_TYPE current_coffee;

SemaphoreHandle_t active_semaphore;

extern TaskHandle_t payment_t;

extern INT8U balance;
extern SemaphoreHandle_t balance_mutex;
//SemaphoreHandle_t turn;
/*****************************   Functions   *******************************/
/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Initialize coffee
 ******************************************************************************/
void coffee_init()
{
    active_semaphore = xSemaphoreCreateBinary();
//    turn =xSemaphoreCreateMutex();
    configASSERT(active_semaphore);

    coffee_types_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(coffee_types_mutex);

    COFFEE_TYPE latte;
    latte.active = 1;
    strcpy(latte.name, "latte");
    latte.price = 26;
    latte.amount_pay = 0;
    latte.grind_time = 5.0f;
    latte.brew_time = 15.0f;
    latte.milk_time = 5.0f;
    coffee_types[1] = latte;

    COFFEE_TYPE espresso;
    espresso.active = 1;
    strcpy(espresso.name, "Espresso");
    espresso.price = 18;
    espresso.amount_pay = 0;
    espresso.grind_time = 8.0f;
    espresso.brew_time = 15.0f;
    espresso.milk_time = 0.0f;
    coffee_types[0] = espresso;

    COFFEE_TYPE filter_coffee;
    filter_coffee.active = 1;
    strcpy(filter_coffee.name, "Filter coffee");
    filter_coffee.price = 1;
    filter_coffee.amount_pay = 1;
    filter_coffee.grind_time = 0.0f;
    filter_coffee.brew_time = 0.0f;
    filter_coffee.milk_time = 0.0f;
    coffee_types[2] = filter_coffee;
}

/*****************************************************************************
 *   Input    : N/A
 *   Output   : Coffee state
 *   Function : Brew state logic / brew coffee
 ******************************************************************************/
COFFEE_STATES brew_state()
{
    FP32 price =
            (current_coffee.amount_pay ? 0.0f : current_coffee.price * 1.0f);
    FP32 slow_dispense = SLOW_DISPENSE_TIME_MS;
    INT8U balance_t = 0;
    INT8U ceil_price = (INT8U) price + 0.5f;
    FP32 inactivity = 0.0f;

    while (1)
    {
        led_off();

        xSemaphoreTake(balance_mutex, portMAX_DELAY);
        balance_t = balance;
        xSemaphoreGive(balance_mutex);

        if (inactivity > MAX_INACTIVITY_MS)
        {
            break;
        }
        else if (ceil_price >= balance_t)
        {
            lprintf(0, "Insert coins");
            if (price != 0.0f && current_coffee.amount_pay)
            {
                inactivity += SWITCH_POLL_DELAY_MS;
            }
        }
        else if (!get_sw2())
        {
            lprintf(0, "Place the cup");
            if (price != 0.0f && current_coffee.amount_pay)
            {
                inactivity += SWITCH_POLL_DELAY_MS;
            }
        }
        else if (!get_sw1())
        {
            lprintf(0, "Hold to start");
            if (price != 0.0f && current_coffee.amount_pay)
            {
                inactivity += SWITCH_POLL_DELAY_MS;
            }
        }
        else
        {
            inactivity = 0.0f;
            if (current_coffee.amount_pay)
            {
                FP32 dispense_mult = 0.0f;
                if (slow_dispense >= 0.0f)
                {
                    slow_dispense -= SWITCH_POLL_DELAY_MS * 1.0f;
                    dispense_mult = SLOW_DISPENSE_AMOUNT;
                }
                else
                {
                    dispense_mult = FAST_DISPENSE_AMOUNT;
                }
                price += (SWITCH_POLL_DELAY_MS / 1000.0f) * current_coffee.price
                        * dispense_mult;
                ceil_price = (INT8U) price + 0.5f;
                led_yellow();
                lprintf(0, "Price: %dkr", ceil_price); // Round up price
            }
            else
            {
                if (current_coffee.grind_time > 0.0f)
                {
                    led_red();
                    lprintf(0, "Grinding");
                    current_coffee.grind_time -= SWITCH_POLL_DELAY_MS / 1000.0f;
                }
                else if (current_coffee.brew_time > 0.0f)
                {
                    led_yellow();
                    lprintf(0, "Brewing");
                    current_coffee.brew_time -= SWITCH_POLL_DELAY_MS / 1000.0f;
                }
                else if (current_coffee.milk_time > 0.0f)
                {
                    led_green();
                    lprintf(0, "Milk frothing");
                    current_coffee.milk_time -= SWITCH_POLL_DELAY_MS / 1000.0f;
                }
                else
                {
                    break;
                }
            }
        }
        vTaskDelay(pdMS_TO_TICKS(SWITCH_POLL_DELAY_MS));
    }

    while (get_sw2())
    {
        lprintf(0, "Remove cup");
        vTaskDelay(pdMS_TO_TICKS(SWITCH_POLL_DELAY_MS));
    }

    xSemaphoreTake(balance_mutex, portMAX_DELAY);
    balance -= ceil_price;
    xSemaphoreGive(balance_mutex);

    current_coffee.price = ceil_price; // For logging

    xSemaphoreGive(active_semaphore);

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Wait for change7

//    xSemaphoreGive(turn);

    return C_LOG;
}

/*****************************************************************************
 *   Input    : N/A
 *   Output   : Coffee state
 *   Function : Select coffee state logic / select coffee
 ******************************************************************************/
COFFEE_STATES select_coffee_state()
{
    lprintf(0, "Pick coffee");

    INT8U num = -1; // To start from 0

    while (1)
    {
        xSemaphoreTake(coffee_types_mutex, portMAX_DELAY);
        do
        {
            if (num < COFFEE_TYPES_LENGTH - 1) num++;
            else num = 0;
        }
        while (!coffee_types[num].active);

        lprintf(1, "%d-%s", num, coffee_types[num].name);

        INT8S inp = key2int(key_get(pdMS_TO_TICKS(COFFEE_CYCLE_TIME_MS)));
        if (inp != -1)
        {
            if (coffee_types[inp].active)
            {
                current_coffee = coffee_types[inp];

                lprintf(0, "price : %d", current_coffee.price);
                lprintf(1, "name  : %s", current_coffee.name);

                INT8U inp = key_get(portMAX_DELAY);
                if (key2int(inp) != -1)
                {
                    // move to payment
                    xTaskNotifyGive(payment_t);

                    xSemaphoreTake(active_semaphore, portMAX_DELAY);

                    xSemaphoreGive(coffee_types_mutex);
                    return BREW;
                }
            }
        }
        xSemaphoreGive(coffee_types_mutex);
    }
}

/*****************************************************************************
 *   Input    : pvParameters
 *   Output   : -
 *   Function : Coffee state machine
 ******************************************************************************/
void coffee_task(void *pvParameters)
{
    COFFEE_STATES current_state = SELECT_COFFEE;
    while (1)
    {
//
////        xSemaphoreTake(turn, portMAX_DELAY);
//        xSemaphoreTake(active_semaphore, portMAX_DELAY);
//
//        COFFEE_TYPE latte;
//        current_coffee.active = 1;
//        strcpy(current_coffee.name, "latte");
//        current_coffee.price = 26;
//        current_coffee.amount_pay = 0;
//        current_coffee.grind_time = 5.0f;
//        current_coffee.brew_time = 15.0f;
//        current_coffee.milk_time = 5.0f;
//        coffee_types[1] = latte;
//
////        current_coffee = latte;
//        brew_state();
        switch (current_state)
        {
        case SELECT_COFFEE:
            current_state = select_coffee_state();
            break;
        case BREW:
//            lprintf(0, "BREW");
//            vTaskDelay(1000);
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
//            lprintf(0, "adfasdfasdf");
            current_state = brew_state();
            break;
        case C_LOG:
            log_coffee(&current_coffee);
            current_state = SELECT_COFFEE;
            break;

        }
    }

}



