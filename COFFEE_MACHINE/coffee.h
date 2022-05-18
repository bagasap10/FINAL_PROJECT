/*
 * coffee.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#ifndef COFFEE_H_
#define COFFEE_H_

/***************************** Include files *******************************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "key.h"
#include "lcd.h"
#include "payment.h"
#include "switch.h"
#include "task.h"
#include "led.h"
/*****************************    Defines    *******************************/
#define COFFEE_TYPES_LENGTH     5 // Allows for one digit types
#define COFFEE_CYCLE_TIME_MS    3000
#define COFFEE_NAME_LENGTH      14

#define SWITCH_POLL_DELAY_MS    500 // Also determines display flash rate
#define SLOW_DISPENSE_TIME_MS   2000.0f
#define SLOW_DISPENSE_AMOUNT    0.5f
#define FAST_DISPENSE_AMOUNT    1.0f
#define MAX_INACTIVITY_MS       5000.0f
/*****************************   Constants   *******************************/
struct COFFEE_TYPE_S
{
    BOOLEAN active;
    char name[COFFEE_NAME_LENGTH]; // Allows it to be displayed
    INT8U price;
    BOOLEAN amount_pay;
    FP32 grind_time;
    FP32 brew_time;
    FP32 milk_time;
};

typedef struct COFFEE_TYPE_S COFFEE_TYPE;
/*****************************   Functions   *******************************/

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Initialize coffee
 ******************************************************************************/
void coffee_init();

/*****************************************************************************
 *   Input    : pvParameters
 *   Output   : -
 *   Function : Coffee state machine
 ******************************************************************************/
void coffee_task(void *pvParameters);

#endif /* COFFEE_H_ */
