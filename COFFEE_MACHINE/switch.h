/*
 * switch.h
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

/***************** Include files **************/
#include <stdint.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"

#ifndef SWITCH_H_
#define SWITCH_H_

/***************** Functions ******************/
void switch_init();
/**********************************************
* Input:
* Output:
* Function:
***********************************************/

INT8U get_sw1();
INT8U get_sw2();

#endif /* SWITCH_H_ */
