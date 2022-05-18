/*
 * led.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "led.h"
/***************** Defines ********************/
/***************** Constants ******************/
/***************** Variables ******************/
/***************** Functions ******************/
void led_init()
{
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R5; // Enable port F

    GPIO_PORTF_DEN_R |= 0x0E; // Enable pins
    GPIO_PORTF_DIR_R |= 0x0E; // Set as output
}
void led_red()
{
    GPIO_PORTF_DATA_R &= ~(0x02);
}

void led_yellow()
{
    GPIO_PORTF_DATA_R &= ~(0x04);
}
void led_green()
{
    GPIO_PORTF_DATA_R &= ~(0x08);
}
void led_off()
{
    GPIO_PORTF_DATA_R |= 0x0E;
}


