/*
 * log.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#ifndef LOG_H_
#define LOG_H_

/***************** Header *********************/
#include "emp_type.h"
#include "coffee.h"
#include "payment.h"
#include "FreeRTOS.h"
#include "queue.h"
#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
/***************** Include files **************/
/***************** Defines ********************/
#define LOG_INPUT_LENGTH    1
#define LOG_COFFEE_WIDTH    sizeof(COFFEE_TYPE)
#define LOG_PAYMENT_WIDTH   sizeof(INT8U)*CARD_LENGTH
#define LOG_LENGTH          10

typedef struct COFFEE_TYPE_S COFFEE_TYPE;

struct LOG_TYPE_S
{
    BOOLEAN active;
    INT8U coffee_number;
    INT8U price;
//    INT8U time;
    char payment_type[CARD_LENGTH + 1]; // Allows for escape char
};

typedef struct LOG_TYPE_S LOG_TYPE;
/***************** Variables ******************/
/***************** Functions ******************/
void log_init();
void log_task(void* pvParameters);
void log_coffee(COFFEE_TYPE* coffee);
void log_payment(INT8U* payment);
void log_time(INT8U* time);
LOG_TYPE* log_nextlog();

#endif /* LOG_H_ */
