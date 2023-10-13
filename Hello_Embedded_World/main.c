/*
 * main.c
 *
 * Created: 8/24/2023 5:26:61 PM
 * Author : tony tim ben
 */ 

//#include <avr/io.h>
#include "board.h"
#include <util/delay.h>
#include "LED.h"
//#include "switch.h"
#include "UART.h"
#include <stdio.h>


// main uses a polling method to listen to all 4 switches, where each LED lights up depending
// on the respective switch pressed.
int main(void)
{
	UART_init(UART0, BAUD_RATE);
	UART_init(UART1, BAUD_RATE);
	UART_init(UART2, BAUD_RATE);
	
	char* print_buffer = export_print_buffer();
	print_buffer = "wow"; 
	sprintf(print_buffer, "print buffer is %f");
	UART_transmit_string()
	//exp 2 inits
	//uint32_t BAUD_RATE = 9600; 

	
	while (1)
	{
		//exp2 - transmits
		uint8_t val = UART_receive(UART1);
		// UART_transmit(UART0, 0x55);
        UART_transmit(UART1, 0x48); //H
        UART_transmit(UART1, 0x65); //e
        UART_transmit(UART1, 0x6C); //l
        UART_transmit(UART1, 0x6C); //l
        UART_transmit(UART1, 0x6F); //o
        UART_transmit(UART1, 0x20); //space
        UART_transmit(UART1, val);
	}
	return 0;
}
