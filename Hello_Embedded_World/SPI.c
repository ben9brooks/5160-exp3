/*
** Author: youngerr bby
** Date: 10/05/1492
*/
#include "SPI.h"
#include "board.h"


unsigned uint8_t round_up_pwr2(unsigned uint8_t n) {
    if (n <= 1) {
        return 1;
    }
    if (n > 128) {
        return 128;
    }
    n--; // the -- ensures that you don't round up when it's already a power of 2.
    n |= n >> 1; // ORs with 1 shift right
    n |= n >> 2; // ORs with 2-bit shift right
    n |= n >> 4; // ORs with 4-bit shift right
    n |= n >> 7; // ORs with 7-bit shit right
    return n + 1; // inc to power of 2 
}

unsigned uint8_t get_spi_prescaler_mask(unsigned uint8_t n) {
    unsigned uint8_t rounded_value = round_up_pwr2(n);
    
    switch (rounded_value) {
        case 4:   return 0b000;
        case 16:  return 0b001;
        case 64:  return 0b010;
        case 128: return 0b011;
        case 2:   return 0b100;
        case 8:   return 0b101;
        case 32:  return 0b110;
        default:  return 0b000; // Default case
    }
}

void SPI_master_init(volatile SPI_t * SPI_addr, uint32_t clock_rate)
{
    // Set CPOL and CPHA to 0
    SPI_addr->control_reg &= (~(3<<2));
    // Enable SPI
    SPI_addr->control_reg |= (1<<6);
    // Set to master mode
    SPI_addr->control_reg |= (1<<4);
    
    // TODO: Set clock rate based on the given `clock_rate`. You can use a series of if-else conditions to check which prescaler value to use.
    uint8_t divider = (F_CPU / OSC_DIV ) / (clock_rate);
    uint8_t mask = get_spi_prescaler_mask(divider);
    SPI_addr->control_reg |= (mask%4); // takes bottom 2 bits or mask & 0x3
    SPI_addr->status_reg |= (mask/4); // takes bit 2

    //add MOSI & SCK pins based on whether it's SPI0 or SPI1
    if(SPI_addr == (SPI_t *) &SPR0)
    {
        PB->DDR_REG |= (1<<5) | (1<<7); //MOSI & SCK Output
        PB->PORT_REG |= (1<<5);  //MOSI 1
        PB->PORT_REG &= ~(1<<7); //SCK 0
    }
    else if (SPI_addr == (SPI_t *) &SPR1)
    {
        PE->DDR_REG |= (1<<3);          // MOSI output
        PD->DDR_REG |= (1<<7);          // SCK output
        PE->PORT_REG |= (1<<3);         // MOSI 1
        PD->PORT_REG &= ~(1<<7);        // SCK 0
    }

}

void SPI_transmit(uint8_t input );
uint8_t SPI_receive(volatile SPI_t *SPI_addr);
uint8_t SPI_transfer(volatile SPI_t *SPI_addr, uint8_t send_value);