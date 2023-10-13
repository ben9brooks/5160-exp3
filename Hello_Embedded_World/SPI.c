/*
** Author: youngerr bby
** Date: 10/05/1492
*/
#include "SPI.h"

void SPI_master_init(volatile SPI_t * SPI_addr, uint32_t clock_rate)
{
    /* Set MOSI and SCK output, all others input */
    // DDR_SPI = (1<<DD_MOSI)|(1<<DD_SCK);
    // offing CPOL and CPHA
    SPI_addr->control_reg &= (~(3<<2));
    // oning peripheral
    SPI_addr->control_reg |= (1<<6);
    // oning MSTR
    SPI_addr->control_reg |= (1<<4);
    
    //TODO: add MOSI pins as output!

    //TODO: add SCK pins

    /* Enable SPI, Master, set clock rate fck/16 */
    SPCR = (1<<SPE)|(1<<MSTR)|(1<<SPR0);

}

void SPI_transmit(uint8_t input );
uint8_t SPI_receive(volatile SPI_t *SPI_addr);
uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t send_value);