
#ifndef SPI_H_
#define SPI_H_ 

#include "board.h"

void SPI_master_init(volatile SPI_t * SPI_addr, uint32_t clock_rate);
void SPI_transmit(uint8_t input );
uint8_t SPI_receive(volatile SPI_t *SPI_addr);
uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t send_value);


#endif 