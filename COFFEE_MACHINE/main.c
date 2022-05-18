

/**
 * main.c
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include "tm4c123gh6pm.h"
#include "emp_type.h"
#include "FreeRTOS.h"
#include "task.h"
#include "uart0.h"
#include "key.h"
#include "systick_frt.h"
#include "digitalswitch.h"
#include "lcd.h"
#include "payment.h"
#include "coffee.h"
#include "led.h"
#include "log.h"
#include "report.h"

extern QueueHandle_t uart0_rx_queue;

TaskHandle_t payment_t;
TaskHandle_t coffee_t;

#define PRIORITY_HIGH 3
#define PRIORITY_MEDIUM 2
#define PRIORITY_LOW 1
#define PRIORITY_IDLE 0

int main(void){
    // Initialize
    uart0_init(19200, 8, 1, 0);
    init_systick();
    key_init();
    digiswitch_init();
    lcd_init();
    payment_init();
    coffee_init();
    switch_init();
    led_init();
    led_off();
    log_init();

    // Create tasks
    xTaskCreate(coffee_task, "Coffee task",
    configMINIMAL_STACK_SIZE + 100,
                NULL, PRIORITY_LOW, &coffee_t);

    xTaskCreate(payment_task, "Payment task",
    configMINIMAL_STACK_SIZE + 100,
                NULL, PRIORITY_LOW, &payment_t);

    xTaskCreate(uart0_write_task, "UART write task",
    configMINIMAL_STACK_SIZE,
                NULL, PRIORITY_LOW, NULL);

    xTaskCreate(key_task, "Key task",
    configMINIMAL_STACK_SIZE,
                NULL, PRIORITY_IDLE, NULL);

    xTaskCreate(digiswitch_task, "Digitalswitch task",
    configMINIMAL_STACK_SIZE,
                NULL, PRIORITY_HIGH, NULL);

    xTaskCreate(lcd_task, "LCD task",
    configMINIMAL_STACK_SIZE + 100,
                NULL, PRIORITY_MEDIUM, NULL);

    xTaskCreate(log_task, "Logger task",
    configMINIMAL_STACK_SIZE,
                NULL, PRIORITY_MEDIUM, NULL);

    xTaskCreate(ui_task, "UI task",
    configMINIMAL_STACK_SIZE + 100,
                NULL, PRIORITY_MEDIUM, NULL);

    vTaskStartScheduler();

    return 1;
}
