#include "board.h"
#include <util/delay.h>
#include "gpio_input.h"

#ifndef SWITCH_H
#define SWITCH_H

#define SW0_port PC 
#define SW0_pin (1<<6)

#define SW1_port PB
#define SW1_pin (1<<2)

#define SW2_port PA
#define SW2_pin (1<<4)

#define SW3_port PA
#define SW3_pin (1<<5)

#define PRESSED 0

// Initializes all 4 switches by using the GPIO_Input_Init function
void switch_Init();
// Reads the current (parameterized) switch, includes a debounce within the conditional statement
uint8_t switch_Read(volatile GPIO_port_t *port_addr, uint8_t pin_mask);

#endif 