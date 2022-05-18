/*
 * lcd.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#ifndef LCD_H_
#define LCD_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "string.h"
#include "FreeRTOS.h"
#include "queue.h"
/*****************************    Defines    *******************************/
// Special ASCII characters
// ------------------------
#define LF      0x0A
#define FF      0x0C
#define CR      0x0D

#define ESC     0x1B

#define LCD_Q_LENGTH 200
#define LCD_Q_WIDTH sizeof(INT8U)

/*****************************   Constants   *******************************/

/*****************************   Functions   *******************************/
void wr_str_LCD(INT8U*);
void move_LCD(INT8U, INT8U);
INT8U wr_ch_LCD(INT8U Ch);

void lcd_init();
void lcd_task(void* pvParameters);
void lprintf(INT16U line, const char * format, ... );

#endif /* LCD_H_ */
