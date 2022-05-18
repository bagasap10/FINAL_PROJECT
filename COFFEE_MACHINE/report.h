/*
 * report.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

/***************** Include files **************/
#include "log.h"
#include "uart0.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
#include "coffee.h"

#ifndef REPORT_H_
#define REPORT_H_

#define ASCII_ESC 27

typedef enum MENU
{
    MAIN_MENU, LOG_LIST, SALES_REPORT
} MENU;
/***************** Variables ******************/
/***************** Functions ******************/
void ui_clear_screen();
void ui_task(void* pvParameters);
MENU main_menu();
MENU log_list_menu();
MENU sales_report_menu();

#endif /* REPORT_H_ */
