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
/*****************************   Functions   *******************************/
/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Initialize coffee
 ******************************************************************************/
void coffee_init()
{
    active_semaphore = xSemaphoreCreateBinary();
    configASSERT(active_semaphore);

    coffee_types_mutex = xSemaphoreCreateMutex();
    xSemaphoreGive(coffee_types_mutex);

    // Init default coffee
    COFFEE_TYPE espresso;
    espresso.active = 1;
    strcpy(espresso.name, "Espresso");
    espresso.price = 15;
    espresso.amount_pay = 0;
    espresso.grind_time = 5.0f;
    espresso.brew_time = 15.0f;
    espresso.milk_time = 0.0f;
    coffee_types[0] = espresso;

    COFFEE_TYPE cappuccino;
    cappuccino.active = 1;
    strcpy(cappuccino.name, "Cappuccino");
    cappuccino.price = 24;
    cappuccino.amount_pay = 0;
    cappuccino.grind_time = 5.0f;
    cappuccino.brew_time = 15.0f;
    cappuccino.milk_time = 3.0f;
    coffee_types[1] = cappuccino;

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
            lprintf(0, "Place cup");
            if (price != 0.0f && current_coffee.amount_pay)
            {
                inactivity += SWITCH_POLL_DELAY_MS;
            }
        }
        else if (!get_sw1())
        {
            lprintf(0, "Hold start");
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
                    lprintf(0, "Grinding...");
                    current_coffee.grind_time -= SWITCH_POLL_DELAY_MS / 1000.0f;
                }
                else if (current_coffee.brew_time > 0.0f)
                {
                    led_yellow();
                    lprintf(0, "Brewing...");
                    current_coffee.brew_time -= SWITCH_POLL_DELAY_MS / 1000.0f;
                }
                else if (current_coffee.milk_time > 0.0f)
                {
                    led_green();
                    lprintf(0, "Milk froth...");
                    current_coffee.milk_time -= SWITCH_POLL_DELAY_MS / 1000.0f;
                }
                else
                {
                    break;
                }
            }
        }
        // Poll switches, this causes the screen to blink
        // to make sure the text is read
        // Definitely a feature and not laziness
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

    ulTaskNotifyTake(pdTRUE, portMAX_DELAY); // Wait for change

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
            if (num < COFFEE_TYPES_LENGTH - 1)
            {
                num++;
            }
            else
            {
                num = 0;
            }
        }
        while (!coffee_types[num].active);

        lprintf(1, "%d-%s", num, coffee_types[num].name);

        INT8S inp = key2int(key_get(pdMS_TO_TICKS(COFFEE_CYCLE_TIME_MS)));
        if (inp != -1)
        {
            if (coffee_types[inp].active)
            {
                current_coffee = coffee_types[inp];

                xTaskNotifyGive(payment_t);

                xSemaphoreTake(active_semaphore, portMAX_DELAY);

                xSemaphoreGive(coffee_types_mutex);
                return BREW;
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
        switch (current_state)
        {
        case SELECT_COFFEE:
            current_state = select_coffee_state();
            break;
        case BREW:
            ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
            current_state = brew_state();
            break;
        case C_LOG:
            log_coffee(&current_coffee);
            current_state = SELECT_COFFEE;
            break;

        }
    }

}



