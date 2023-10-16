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
	// print_buffer = "wow"; 
	// sprintf(print_buffer, "print buffer is %f");
	// UART_transmit_string();
	//exp 2 inits
	UART_init(UART0, BAUD_RATE);
	UART_init(UART1, BAUD_RATE);
	UART_init(UART2, BAUD_RATE);
	char* print_buffer = export_print_buffer();

	//exp3 inits
	SPI_master_init(SPI0, 2500000U); // 0 is used for SD card, OLED, MP3
	SPI_master_init(SPI1, 2500000U);
	uint8_t data = 0x52;
	uint8_t error =  0;

	
	while (1)
	{
		//exp2 - transmits
		//uint8_t val = UART_receive(UART1);
		error = SPI_transfer(SPI0, 0xAC, &data);

        // UART_transmit(UART1, 'h');
		// while(1)
		// {
			//error = SPI_transmit(SPI0, 0x4C, &data);
			//error = SPI_transmit(SPI0, 0x4C, &data);
			// error = SPI_transfer(SPI0, 0x4C, &data);
			// if(error != 0)
			// {
			// 	UART_transmit(UART1, error);
			// }
			// if(error == 0)
			// {
			// 	//succcessful
			// 	UART_transmit(UART1, 0x2b); // '+'
			// }
			
			//if ( SPI_transmit(SPI1, 0x10, &data) != 0)
			//{
			//	break;
			//}
			//UART_transmit(UART1, '.');
			//if(SPI_receive(SPI1, &data) != 0)
			//{
			//	break;
			//}
		// }
		
		// UART_transmit(UART1, 'i');
	}
	return 0;
}
