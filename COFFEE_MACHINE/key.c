/*
 * key.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "key.h"

QueueHandle_t Q_KEY;

INT8U row(INT8U y)
{
    INT8U result = 0;

    switch (y)
    {
    case 0x01:
        result = 1;
        break;
    case 0x02:
        result = 2;
        break;
    case 0x04:
        result = 3;
        break;
    case 0x08:
        result = 4;
        break;
    }
    return (result);
}

INT8U key_catch(x, y)
    INT8U x, y;
{
    const INT8U matrix[3][4] = { { '*', '7', '4', '1' }, { '0', '8', '5', '2' },
                                 { '#', '9', '6', '3' } };

    return (matrix[x - 1][y - 1]);
}

INT8U key_get(TickType_t xTicksToWait)
{
    INT8U key;
    if (xQueueReceive(Q_KEY, &key, xTicksToWait))
    {
        return key;
    }
    return 0;
}

BOOLEAN check_column(INT8U x)
{
    INT8U y = GPIO_PORTE_DATA_R & 0x0F; // Save the values of the 4 bits for the rows
    if (y)
    {
        INT8U ch = key_catch(x, row(y));
        xQueueSend(Q_KEY, &ch, 0);
        return 1;
    }
    return 0;
}

void key_init()
{
    int dummy;

    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0 | SYSCTL_RCGCGPIO_R4; // Enable port A and E

    dummy = SYSCTL_RCGC2_R; // Wait for clock enable

    // Set columns as output
    GPIO_PORTA_DIR_R |= 0x1C;

    // Enable pins for rows and columns
    GPIO_PORTA_DEN_R |= 0x1C;
    GPIO_PORTE_DEN_R |= 0x0F;

    Q_KEY = xQueueCreate(KEY_Q_LENGTH, KEY_Q_WIDTH);
    configASSERT(Q_KEY);
}

void key_task()
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function :
 ******************************************************************************/
{
    INT8U my_state = 0;
    while (1)
    {
        switch (my_state)
        {
        case 0:
            GPIO_PORTA_DATA_R &= 0xE3;       // Clear the 3 bits for the columns
            GPIO_PORTA_DATA_R |= 0x10;          // Set the bit for column 1
            if (check_column(1)) // Check all the rows for column 1, using the function check_column
            { // If a button press is registered we go to next state so the press is only registered once
                my_state = 1;
                break;
            }
            GPIO_PORTA_DATA_R &= 0xE3; // Repeat the above for the two other columns
            GPIO_PORTA_DATA_R |= 0x08;
            if (check_column(2))
            {
                my_state = 1;
                break;
            }
            GPIO_PORTA_DATA_R &= 0xE3;
            GPIO_PORTA_DATA_R |= 0x04;
            if (check_column(3))
            {
                my_state = 1;
                break;
            }
            break;
        case 1:
            // We stay here until the button is released so a button press is not counted more than once
            if (!(GPIO_PORTE_DATA_R & 0x0F))
            {
                // Wait for debounce
                vTaskDelay(pdMS_TO_TICKS(KEY_DEBOUNCE_DELAY_MS));
                // Switch back
                my_state = 0;
            }
            break;
        }
    }
}



