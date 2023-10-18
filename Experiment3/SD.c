/*
 * SD.c
 *
 * Created: 10/16/2023 5:03:32 PM
 *  Author: Ben
 */ 
 #include "board.h"
 #include "SPI.h"
 #include "SD.h"
#include <util/delay.h>


#define SD_CS_port (PB) //(&PINB)
#define SD_CS_pin (1<<4)

void SD_CS_active(volatile uint8_t *port, uint8_t pin);
void SD_CS_inactive(volatile uint8_t *port, uint8_t pin);

void SD_CS_active(volatile uint8_t *port, uint8_t pin)
{
	GPIO_Output_Clear(port,pin);
}
void SD_CS_inactive(volatile uint8_t *port, uint8_t pin)
{
	GPIO_Output_Set(port,pin);
}

uint8_t send_command (volatile SPI_t *SPI_addr, uint8_t command, uint32_t argument)
{
	//Maybe we make a separate function call to return an error? seems like error-checking is common and there should be a clean solution.
	uint8_t errorStatus = 0; // No error by default
    uint8_t checksum = 0x01; // Default checksum value
    uint8_t data; // Placeholder for received SPI data

	//1: check if command is 6 bits (<= 63). If not, error flag & function exits.
	if (command > 63)
	{
		//change this?
		return ERROR_SPI;
	}
	//2: command OR'd with 0x40 to append start and transmission bits to the first byte to send.
	command |= 0x40;
	//3: Send first byte using SPI_transfer. If error found from transfer, exit.
	errorStatus = SPI_transfer(SPI_addr, command, &data);
	if (errorStatus != 0) return errorStatus;
	//4: 32-bit arg sent, MSB first. Exit if error occurs.
	for (int i = 3; i >= 0; i--) // Start from the MSB, i starts high
    {
		//this shifts right in multiples of 8. Since we can only send 8 bits, the first one (i=3) is shifted right 24b, sending the 8 MSBs first.
        errorStatus = SPI_transfer(SPI_addr, (argument >> (8 * i)) & 0xFF, &data);
        if (errorStatus != 0) return errorStatus;
    }
	//5: checksum byte, lsb set to 1. If cmd is 0 or 8, checksum must be sent, otherwise 0x01 can be sent.
	//CMD0: 01 000000  or 0x40 in the first byte. We OR'd 0x40 so it's 01 and then the remaining 6 determines the cmd.
	//CMD8: 01 001000  or 0x48
	if (command == 0x40)
	{
		//CMD 0
		checksum = 0x95;
	}
	if (command == 0x48)
	{
		//CMD 8
		checksum = 0x87; //found in notes
	}

	// data is overwritten here, does that matter?
	errorStatus = SPI_transfer(SPI_addr, checksum, &data);
	
	//6: return error status
	return errorStatus;
}

// !!! The array_name parameter, when used, must be an array of defined size!
uint8_t receive_response (volatile SPI_t *SPI_addr, uint8_t number_of_bytes, uint8_t * array)
{
	uint8_t errorStatus = 0;
	uint8_t timeout = 0;
	uint8_t rcvd_val;
	uint8_t data;
	//size of response varies, can be 1-5 bytes. Response has short delay, 
	// 1. send 0xFF repeatedly, and keep reading the received value. This is all done using SPI_transfer. 
	//    continue until msb of received byte is 0 or timeout on the loop. If timed out, return error and send 0xFF.
	do
	{
		rcvd_val = SPI_transfer(SPI_addr, 0xFF, &data); //SPI receive?
		timeout++;
	} while ( (rcvd_val == 0xFF) && (timeout != 0) );
	// handle timeout errors:
	if (timeout == 0)
	{
		return ERROR_TIMEOUT;
	}
	else if ( (rcvd_val & 0xFE)	!= 0x00 ) //0x00 and 0x01 are good values
	{
		*array = rcvd_val; //return value to see error
		return ERROR_SD;
	}
	else
	{
		//receive the remainder of the bytes, if present.
		// 2. If more than one byte expected, 0xFF sent out and each received byte stored in array. Repeat until all bytes received.
		*array = rcvd_val;
		if(number_of_bytes>1)
		{
			//start at 1 bc just got index 0, 3 lines above this
			for(uint8_t i = 1; i < number_of_bytes; i++)
			{
				rcvd_val = SPI_transfer(SPI_addr, 0xFF, &data);
				array[i] = rcvd_val;
			}
		}
	}
	
	// 3. an additional 0xFF byte should be sent after the entire response. Received value is irrelevant.
	rcvd_val = SPI_transfer(SPI_addr, 0xFF, &data);
	// 4. return error value
	return 0;
}

 void SD_init(volatile SPI_t *SPI_addr)
 {
	 /* ---- Extra Notes
	    initialize sd - set SS high, send 10 spi transfers for 80 clocks
	    send ss low, send CMD0,
	    PB4
	    ------ */
	
	 //init spi to master mode (can this be done externally?)
	 
	 uint8_t errorStatus = 0;
	 uint8_t data = 0;
	 uint32_t arg = 0x00000000;
	 uint8_t response_cmd0[1] = {0};
     uint8_t response_cmd8[5] = {0,0,0,0,0};
	 uint32_t ACMD41_arg = 0x00000000;
	 
	 //set SS to 1 (which is PB4)
	 GPIO_Output_Init(PB, (1<<4));
	 GPIO_Output_Set(PB, (1<<4));
	 //send 80 clock-cycles worth of transmits 
	 for(uint8_t i = 0; i < 8; i++)
	 {
		 errorStatus = SPI_transfer(SPI_addr, 0xFF, &data);
	 }

	 //set SS to 0
	 GPIO_Output_Clear(PB, (1<<4));
	 
	 //send CMD0, expecting R1. If not R1, stop here.
	 errorStatus = send_command(SPI_addr, CMD0, arg);
	 if (errorStatus == 0)
	 {
		 errorStatus = receive_response(SPI_addr, 1, &response_cmd0);
	 }
	 if(response_cmd0[0] != 0x01)
	 {
		 return ERROR_SD;
	 }

	 //send CM8, expecting R7. If voltage val != 0x01 or if check byte doesn't match, stop here.
	 errorStatus = send_command(SPI_addr, CMD8, 0x000001AA);
	 if(errorStatus == 0)
	 {
		errorStatus = receive_response(SPI_addr, 5, &response_cmd8);
	 }
	 
	 // if reponse is 0x05 (illegal cmd), flag it for later, bc it can't be high capacity (SDHC).
	 if((response_cmd8[0] == 0x01) && (errorStatus == 0))
	 {
		if((response_cmd8[3] == 0x01 ) && (response_cmd8[4] == 0xAA))
		{
			ACMD41_arg = 0x40000000; //high voltage 
		}
		else
		{
			return ERROR_VOLTAGE;
		}
	 }
	 else if(response_cmd8[0] == 0x05)
	 {
		ACMD41_arg = 0x00000000;
		//sd_card_type = ??
	 }
	 else
	 {
		return ERROR_SD;
	 }

	 // turn CS high?
	 GPIO_Output_Set(PB, (1<<4));

	 return 0;
 }