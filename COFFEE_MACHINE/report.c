/*
 * report.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "report.h"
/***************** Defines ********************/

/***************** Constants ******************/
/***************** Variables ******************/
extern COFFEE_TYPE coffee_types[COFFEE_TYPES_LENGTH];
extern SemaphoreHandle_t coffee_types_mutex;

extern LOG_TYPE log_array[LOG_LENGTH];
extern SemaphoreHandle_t log_array_semaphore;

extern QueueHandle_t uart0_rx_queue;

char buffer[70];
/***************** Functions ******************/

void ui_clear_screen()
{
    char buffer[5];
    uprintf(buffer, "%c[2J", ASCII_ESC); // Clear screen
    uprintf(buffer, "%c[H", ASCII_ESC); // Reset cursor
}

void ui_task(void *pvParameters)
{
    MENU current_menu = MAIN_MENU;

    while (1)
    {
        switch (current_menu)
        {
        case MAIN_MENU:
            current_menu = main_menu();
            break;
        case LOG_LIST:
            current_menu = log_list_menu();
            break;
        case SALES_REPORT:
            current_menu = sales_report_menu();
            break;
        }
    }
}

MENU main_menu()
{
    ui_clear_screen();
    uprintf(buffer, "----------------------\n\r");
    uprintf(buffer, "Select menu\n\r");
    uprintf(buffer, "1 - List logs\n\r");
    uprintf(buffer, "2 - Sales report\n\r");
    uprintf(buffer, "----------------------\n\r");

    INT8U inp;
    while (1)
    {
        xQueueReceive(uart0_rx_queue, &inp, portMAX_DELAY);
        switch (inp)
        {
        case '1':
            return LOG_LIST;
        case '2':
            return SALES_REPORT;
        default:
            uprintf(buffer, "%c\n\rIncorrect input\n\r", inp);
            break;
        }
    }
}

MENU log_list_menu()
{
    int i;
    ui_clear_screen();
    uprintf(buffer,
            "Order no. | Coffee no. | Coffee name   | Price | Payment type\n\r");

    xSemaphoreTake(log_array_semaphore, portMAX_DELAY);
    xSemaphoreTake(coffee_types_mutex, portMAX_DELAY);

    for (i = 0; i < LOG_LENGTH; i++)
    {
        if (log_array[i].active)
        {
            uprintf(buffer, "%-10d| %-11d| %-14s| %-6d| %s\n\r", i,
                    log_array[i].coffee_number,
                    coffee_types[log_array[i].coffee_number].name,
                    log_array[i].price, log_array[i].payment_type);
        }
        else
        {
            break;
        }
    }

    xSemaphoreGive(log_array_semaphore);
    xSemaphoreGive(coffee_types_mutex);

    uprintf(buffer, "Press any key to return\n\r");
    INT16U inp = 0;
    xQueueReceive(uart0_rx_queue, &inp, portMAX_DELAY);
    uprintf(buffer, "%d", inp);

    return MAIN_MENU;
}

MENU sales_report_menu()
{
    int i, j;
    ui_clear_screen();
    uprintf(buffer, "Coffee name   | Total sales\n\r");

    xSemaphoreTake(log_array_semaphore, portMAX_DELAY);
    xSemaphoreTake(coffee_types_mutex, portMAX_DELAY);

    for (i = 0; i < COFFEE_TYPES_LENGTH; i++)
    {
        if (coffee_types[i].active)
        {
            INT16U sum = 0;
            for (j = 0; j < LOG_LENGTH; j++)
            {
                if (log_array[j].active)
                {
                    if (i == log_array[j].coffee_number)
                    {
                        sum += log_array[j].price;
                        break;
                    }
                }
                else
                {
                    break;
                }
            }
            uprintf(buffer, "%-14s| %d\n\r", coffee_types[i].name, sum);
        }
        else
        {
            break;
        }
    }

    xSemaphoreGive(log_array_semaphore);
    xSemaphoreGive(coffee_types_mutex);

    uprintf(buffer, "Press any key to return\n\r");
    INT16U inp = 0;
    xQueueReceive(uart0_rx_queue, &inp, portMAX_DELAY);
    uprintf(buffer, "%d", inp);

    return MAIN_MENU;
}


