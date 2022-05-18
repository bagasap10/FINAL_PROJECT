/*
 * uart0.c
 *
 *  Created on: May 18, 2022
 *      Author: bagas
 */

#include "uart0.h"
/*****************************    Defines    *******************************/

/*****************************   Constants   *******************************/

/*****************************   Variables   *******************************/
QueueHandle_t uart0_rx_queue;
QueueHandle_t uart0_tx_queue;
/*****************************   Functions   *******************************/

INT32U lcrh_databits(INT8U antal_databits)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : sets bit 5 and 6 according to the wanted number of data bits.
 *              5: bit5 = 0, bit6 = 0.
 *              6: bit5 = 1, bit6 = 0.
 *              7: bit5 = 0, bit6 = 1.
 *              8: bit5 = 1, bit6 = 1  (default).
 *             all other bits are returned = 0
 ******************************************************************************/
{
    if ((antal_databits < 5) || (antal_databits > 8))
        antal_databits = 8;
    return (((INT32U) antal_databits - 5) << 5);  // Control bit 5-6, WLEN
}

INT32U lcrh_stopbits(INT8U antal_stopbits)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : sets bit 3 according to the wanted number of stop bits.
 *              1 stpobit:  bit3 = 0 (default).
 *              2 stopbits: bit3 = 1.
 *             all other bits are returned = 0
 ******************************************************************************/
{
    if (antal_stopbits == 2)
        return (0x00000008);        // return bit 3 = 1
    else
        return (0x00000000);        // return all zeros
}

INT32U lcrh_parity(INT8U parity)
/*****************************************************************************
 *   Input    :
 *   Output   :
 *   Function : sets bit 1, 2 and 7 to the wanted parity.
 *              'e':  00000110b.
 *              'o':  00000010b.
 *              '0':  10000110b.
 *              '1':  10000010b.
 *              'n':  00000000b.
 *             all other bits are returned = 0
 ******************************************************************************/
{
    INT32U result;

    switch (parity)
    {
    case 'e':
        result = 0x00000006;
        break;
    case 'o':
        result = 0x00000002;
        break;
    case '0':
        result = 0x00000086;
        break;
    case '1':
        result = 0x00000082;
        break;
    case 'n':
    default:
        result = 0x00000000;
    }
    return (result);
}

extern BOOLEAN uart0_tx_rdy()
{
    return ( UART0_FR_R & UART_FR_TXFE);
}

extern void uart0_init(INT32U baud_rate, INT8U databits, INT8U stopbits,
                       INT8U parity)
/*****************************************************************************
 *   Function : See module specification (.h-file).
 *****************************************************************************/
{
    INT32U BRD;

    // Enable clock for UART and GPIO port A
    SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R0; // Enable clock for Port A
    SYSCTL_RCGCUART_R |= SYSCTL_RCGCUART_R0; // Enable UART 0

    // Setup GPIO PA0 and PA1
    GPIO_PORTA_AFSEL_R |= 0x00000003; // set PA0 og PA1 to alternativ function (uart0)
    GPIO_PORTA_DIR_R |= 0x00000002;  // set PA1 (uart0 tx) to output
    GPIO_PORTA_DIR_R &= ~(0x00000002); // set PA0 (uart0 rx) to input
    GPIO_PORTA_DEN_R |= 0x00000003;  // enable digital operation of PA0 and PA1

    // Calculate baudrate prescaler
    BRD = 64000000 / baud_rate; // X-sys*64/(16*baudrate) = 16M*4/baudrate
    UART0_IBRD_R = BRD / 64;
    UART0_FBRD_R = BRD & 0x0000003F;

    // Setup uart with 8 databits,
    UART0_LCRH_R = lcrh_databits(databits);
    UART0_LCRH_R += lcrh_stopbits(stopbits);
    UART0_LCRH_R += lcrh_parity(parity);

    //UART0_LCRH_R |= UART_LCRH_FEN; // FIFO Enable

    UART0_CTL_R |= UART_CTL_UARTEN | UART_CTL_TXE | UART_CTL_RXE;  // Enable UART

    // Setup receive interrupt
    UART0_IM_R |= UART_IM_RTIM; // Interrupt mask

    // NVIC Enable interrupt 5 (UART0)
    NVIC_EN0_R |= (1 << 5);
    // Set next highest priority (lowest numberical value) allowed by FreeRTOS
    NVIC_PRI1_R |= (101 << 13);

    // Setup rx and tx queues
    uart0_rx_queue = xQueueCreate(UART_QUEUE_LENGTH, UART_ITEM_SIZE);
    configASSERT(uart0_rx_queue);

    uart0_tx_queue = xQueueCreate(UART_QUEUE_LENGTH, UART_ITEM_SIZE);
    configASSERT(uart0_tx_queue);
}

void uart0_read_isr()
{

    while (!(UART0_FR_R & UART_FR_RXFE))
    {
        // Make sure no errors in transmission except overrun (triggers with values like æøå)
        configASSERT(!(UART0_DR_R & 0x700));
        // Receive msg
        INT8U msg = UART0_DR_R & 0xFF;
        // Remove from queue if full
        if (xQueueIsQueueFullFromISR(uart0_rx_queue)) {
            INT8U remmsg;
            xQueueReceiveFromISR(uart0_rx_queue, &remmsg, NULL);
        }
        // Add to queue
        xQueueSendToBackFromISR(uart0_rx_queue, &msg, NULL);
    }
}

void uart0_write_task(void * pvParameters)
{
    while (1)
    {
        INT8U msg;
        // Wait till message appears in queue
        xQueueReceive(uart0_tx_queue, &msg, portMAX_DELAY);

        // Poll till transmit FIFO not full
        while (UART0_FR_R & UART_FR_TXFF) {

        }
        // Transmit msg
        UART0_DR_R = msg;
    }
}

void uart0_sendstring(char* c, INT8U length)
{
    int i;
    for (i = 0; i < length; i++)
    {
        xQueueSendToBack(uart0_tx_queue, &c[i], portMAX_DELAY);
    }
}

void uprintf(char* buffer, const char * format, ... )
{
    va_list args;
    va_start(args, format);
    INT8U len = vsprintf(buffer, format, args);
    va_end(args);
    uart0_sendstring(buffer, len);
    return;
}


