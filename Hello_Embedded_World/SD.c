/*
 * SD.c
 *
 * Created: 10/16/2023 5:03:32 PM
 *  Author: Ben
 */ 

uint8_t send_command (volatile SPI_t *SPI_addr, uint8_t command, uint32_t argument)
{
	//Maybe we make a separate function call to return an error? seems like error-checking is common and there should be a clean solution.
	
	//1: check if command is 6 bits (<= 63). If not, error flag & function exits.
	
	//2: command OR'd with 0x40 to append start and transmission bits to the first byte to send.
	
	//3: Send first byte using SPI_transfer. If error found from transfer, exit.
	
	//4: 32-bit arg sent, MST first. Exit if error occurs.
	
	//5: checksum byte, lsb set to 1. If cmd is 0 or 8, checksum must be sent, otherwise 0x01 can be sent.
	
	//6: return error status
}

uint8_t receive_response (volatile SPI_t *SPI_addr, uint8_t number_of_bytes, uint8_t * array_name)
{
	//size of response varies, can be 1-5 bytes. Response has short delay, 
	// 1. send 0xFF repeatedly, and keep reading the received value. This is all done using SPI_transfer. 
	//    continue until msb of received byte is 0 or timeout on the loop. If timed out, return error and send 0xFF.
	
	// 2. If more than one byte expected, 0xFF sent out and each received byte stored in array. Repeat until all bytes received.
	
	// 3. an additional 0xFF byte should be sent after the entire response. Received value is irrelevant.
	
	// 4. return error value
}