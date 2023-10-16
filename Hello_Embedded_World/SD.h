/*
 * SD.h
 *
 * Created: 10/16/2023 5:03:46 PM
 *  Author: Ben
 */ 


#ifndef SD_H_
#define SD_H_


uint8_t send_command (volatile SPI_t *SPI_addr, uint8_t command, uint32_t argument);
uint8_t receive_response (volatile SPI_t *SPI_addr, uint8_t number_of_bytes, uint8_t * array_name);


#endif /* SD_H_ */