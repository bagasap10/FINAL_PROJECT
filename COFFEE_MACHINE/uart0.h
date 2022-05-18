/*
 * uart0.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#ifndef UART0_H_
#define UART0_H_

/***************************** Include files *******************************/
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"
/*****************************    Defines    *******************************/
#define UART_QUEUE_LENGTH 16
#define UART_ITEM_SIZE    8
/***************** Variables ******************/
extern QueueHandle_t uart0_rx_queue;
extern QueueHandle_t uart0_tx_queue;
/*****************************   Functions   *******************************/

BOOLEAN uart0_rx_rdy();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Character ready at uart0 RX
******************************************************************************/

INT8U uart0_getc();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Get character from uart0 RX
******************************************************************************/

BOOLEAN uart0_tx_rdy();
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : uart0 TX buffer ready
******************************************************************************/

void uart0_putc( INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Put character to uart0 TX
******************************************************************************/

void uart0_init( INT32U, INT8U, INT8U, INT8U );
/*****************************************************************************
*   Input    : -
*   Output   : -
*   Function : Initialize uart 0
******************************************************************************/

void uart0_read_isr();
void uart0_write_task(void* pvParameters);
void uart0_sendstring(char* c, INT8U length);
void uprintf(char* str, const char * format, ... );

#endif /* UART0_H_ */
