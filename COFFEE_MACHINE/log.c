/*
 * log.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "log.h"
/***************** Defines ********************/
/***************** Constants ******************/
/***************** Variables ******************/
QueueHandle_t log_coffee_q;
QueueHandle_t log_payment_q;

LOG_TYPE log_array[LOG_LENGTH];
SemaphoreHandle_t log_array_semaphore;

extern COFFEE_TYPE coffee_types[COFFEE_TYPES_LENGTH];
extern SemaphoreHandle_t coffee_types_mutex;
/***************** Functions ******************/
void log_init()
{
    // Create queues
    log_coffee_q = xQueueCreate(LOG_INPUT_LENGTH, LOG_COFFEE_WIDTH);
    configASSERT(log_coffee_q);

    log_payment_q = xQueueCreate(LOG_INPUT_LENGTH, LOG_PAYMENT_WIDTH);
    configASSERT(log_payment_q);

    // Create semaphore for array
    log_array_semaphore = xSemaphoreCreateMutex();
    xSemaphoreGive(log_array_semaphore);
}

void log_task(void* pvParameters)
{
    COFFEE_TYPE coffee_type;
    INT8U payment_type[CARD_LENGTH];
    INT8U i;
    //int i;
    while (1)
    {
        xQueueReceive(log_coffee_q, &coffee_type, portMAX_DELAY);
        xQueueReceive(log_payment_q, &payment_type, portMAX_DELAY);

        xSemaphoreTake(log_array_semaphore, portMAX_DELAY);

        LOG_TYPE* log = log_nextlog();

        log->active = 1;
        log->price = coffee_type.price;

        // Convert payment to string
        if (payment_type[0] == CASH_ID)
        {
            strcpy(&log->payment_type, "CASH    ");
        }
        else
        {
            for (i = 0; i < CARD_LENGTH; i++)
            {
                log->payment_type[i] = payment_type[i] + '0';
            }
        }

        // Get coffee number
        xSemaphoreTake(coffee_types_mutex, portMAX_DELAY);
        INT8U num = -1;
        for (i = 0; i < COFFEE_TYPES_LENGTH; i++)
        {
            if (!strcmp(coffee_types[i].name, coffee_type.name))
            { // Names are equal
                num = i;
                break;
            }
        }
        xSemaphoreGive(coffee_types_mutex);

        configASSERT(num != -1);

        log->coffee_number = num;

        xSemaphoreGive(log_array_semaphore);
    }
}

void log_coffee(COFFEE_TYPE* coffee)
{
    xQueueSendToBack(log_coffee_q, coffee, portMAX_DELAY);
}

void log_payment(INT8U* payment_type)
{
    xQueueSendToBack(log_payment_q, payment_type, portMAX_DELAY);
}

LOG_TYPE* log_nextlog()
{
    int i;
    for(i = 0; i < LOG_LENGTH; i++)
    {
        if (!log_array[i].active)
        {
            return &log_array[i];
        }
    }
    return NULL;
}


