/*
 * key.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "task.h"

#ifndef KEY_H_
#define KEY_H_

#define KEY_Q_WIDTH sizeof(INT8U)
#define KEY_Q_LENGTH 10
#define KEY_DEBOUNCE_DELAY_MS 10

INT8U key_get(TickType_t xTicksToWait);
void key_task();
void key_init();

#endif /* KEY_H_ */
