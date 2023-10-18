/*
 * main.c
 *
 * Created: 8/24/2023 5:26:61 PM
 * Author : tony tim ben
 */ 

//#include <avr/io.h>
#include "board.h"
#include <util/delay.h>
#include "UART.h"
#include <stdio.h>
#include "SD.h"


int main(void)
{
	//exp 2 inits
	UART_init(UART0, BAUD_RATE);
	UART_init(UART1, BAUD_RATE);
	UART_init(UART2, BAUD_RATE);

	//exp3 inits
	SPI_master_init(SPI0, 400000U); // 0 is used for SD card, OLED, MP3
	//SPI_master_init(SPI1, 2500000U);
	uint8_t data = 0x52;
	uint8_t error =  0;
	uint32_t arg = 0x00000000;
	enum ErrorTypes typederror = 0;
	
	
	char start[] = "Start\n";
	char stop[] = "Stop\n";
	
	// initialize SS AKA CS
	GPIO_Output_Init(PB, (1<<4));
	
	UART_transmit_string(UART1, start, 6);
	
	typederror = SD_init(SPI0);
	if (typederror != 0)
	{
		display_error(UART1, typederror);
	}
	

	UART_transmit_string(UART1, "SD initialized\n", 15);
	while (1)
	{
		// to debug SPI_transmit on MSO: Trigger Menu: Type=Edge, Source=D0, Slope=All, Level=1.51, Normal
		//error = SPI_transmit(SPI0, 0xFF, data);
		//GPIO_Output_Clear(PB, (1<<4));
		
		
		error = SPI_transfer(SPI0, 0xFF, &data);
		//error = send_command(SPI0, CMD0, arg);
		if(error != 0)
		{
			break;
		}
		//error = SPI_receive(SPI0, &data);
		if(error != 0)
		{
			break;
		}
	}
	
	UART_transmit_string(UART1, stop, 5);
	UART_transmit(UART1, data);
	UART_transmit(UART1, '\n');
	return 0;
}
