/*
 * payment.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

/***************************** Include files *******************************/
#include <stdint.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "key.h"
#include "lcd.h"
#include "semphr.h"
#include "queue.h"
#include "led.h"
#include "log.h"
/*****************************    Defines    *******************************/
#define CARD_METHOD            '1'
#define CASH_METHOD            '2'
#define CARD_LENGTH             16
#define PIN_LENGTH              4
#define CARD_MAX_ATTEMPTS       3
#define DEFAULT_CARD            26
#define CLOCKWISE               20
#define COUNTERCLOCKWISE        5
#define CASH_ID                 10
#define SWITCH_POLL_DELAY_MS    500

#ifndef PAYMENT_H_
#define PAYMENT_H_

/***************** Variables ******************/
typedef enum PAYMENT_STATES
{
    START, PAYMENT, CARD, CASH, CARD_NUMBER, PIN, LOG, CHANGE
} PAYMENT_STATES;

/*****************************   Constants   *******************************/
/*****************************   Functions   *******************************/

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Init payment
 ******************************************************************************/
void payment_init();

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Payment state machine
 ******************************************************************************/
void payment_task(void* pvParameters);

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Cash state logic / calculate difference out on led
 ******************************************************************************/
PAYMENT_STATES change_state();

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Paymenttype logic / choice of payment type
 ******************************************************************************/
PAYMENT_STATES paymenttype_state();

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Cardnumber state logic / check for 8 digit cardnumber
 ******************************************************************************/
PAYMENT_STATES cardnumber_check_state();

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Pin state logic / check pin
 ******************************************************************************/
PAYMENT_STATES pin_check_state();

/*****************************************************************************
 *   Input    : N/A
 *   Output   : -
 *   Function : Cash state logic / update payment balance
 ******************************************************************************/
PAYMENT_STATES cash_state();

/*****************************************************************************
 *   Input    : INT8U
 *   Output   : INT8S
 *   Function : Convert ascii key to int
 ******************************************************************************/
INT8S key2int(INT8U key);

#endif /* PAYMENT_H_ */
