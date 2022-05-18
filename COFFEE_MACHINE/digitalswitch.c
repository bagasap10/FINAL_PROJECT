/*
 * digitalswitch.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

/***************** Include files **************/
#include "digitalswitch.h"
/***************** Defines ********************/
/***************** Constants ******************/
/***************** Variables ******************/
TaskHandle_t digiswitch_t;
QueueHandle_t ds_input_queue;
/***************** Functions ******************/
/**********************************************
 * Input:
 * Output:
 * Function:
 ***********************************************/

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Initialize digiswitch
 ******************************************************************************/
void digiswitch_init()
{
    int dummy;

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // Enable port A

    dummy = SYSCTL_RCGC2_R; // Wait for clock enable

    // Enable pins
    GPIO_PORTA_DEN_R |= 0x60;

    // Interrupt on A (PA5)
    GPIO_PORTA_IS_R &= ~(0b00100000); // Edge trigger
    GPIO_PORTA_IBE_R |= 0b00100000; // Both edges
    GPIO_PORTA_IM_R |= 0b00100000; // Interrupt mask

    // NVIC Enable interrupt 0 (GPIOA)
    NVIC_EN0_R |= (1 << 0);
    // Set next highest priority (lowest numberical value) allowed by FreeRTOS
    NVIC_PRI0_R |= (101 << 5);

    // Create input queue
    ds_input_queue = xQueueCreate(DS_INPUT_QUEUE_LENGTH, DS_INPUT_QUEUE_WIDTH);
    configASSERT(ds_input_queue);
}

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Digiswitch ISR
 ******************************************************************************/
void digiswitch_isr()
{
    if (GPIO_PORTA_RIS_R & 0b00100000)
    {
        // Disable interrupt
        GPIO_PORTA_IM_R &= ~(0b00100000);

        INT8S dir = 0;
        // Rotation dependent on falling or rising edge
        // and the value of B (PA6)
        if (GPIO_PORTA_DATA_R & 0b00100000) // Rising edge
        {
            if (GPIO_PORTA_DATA_R & 0b01000000)
            {
                dir = -1;
            }
            else
            {
                dir = 1;
            }
        }
        else // Faling edge
        {
            if (GPIO_PORTA_DATA_R & 0b01000000)
            {
                dir = 1;
            }
            else
            {
                dir = -1;
            }
        }

        xQueueSendToBackFromISR(ds_input_queue, &dir, NULL);

        vTaskNotifyGiveFromISR(digiswitch_t, NULL);
    }
}

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Digiswitch FreeRTOS task
 ******************************************************************************/
void digiswitch_task(void* pvParameters)
{
    digiswitch_t = xTaskGetCurrentTaskHandle();

    while (1)
    {
        // Clear and enable interrupt
        GPIO_PORTA_ICR_R |= 0b00100000;
        GPIO_PORTA_IM_R |= 0b00100000;

        // Wait debounce
        vTaskDelay(pdMS_TO_TICKS(DS_DEBOUNCE_MS));

        // Wait for next interrupt
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);
    }
}

/*****************************************************************************
 *   Input    : TickType
 *   Output   : Direction of turn
 *   Function : Get direction of turn and add to array
 ******************************************************************************/
INT8S digiswitch_get(TickType_t xTicksToWait)
{
    INT8S dir;
    if (xQueueReceive(ds_input_queue, &dir, xTicksToWait) == 0) {
        dir = 0;
    }
    return dir;
}


