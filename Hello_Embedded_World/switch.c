/*
 * switch.h
 *
 * Created: 9/11/2023 1:38:42 PM
 *  Author: tim tony ben
 */

#include "switch.h"
 
// Initializes all 4 switches by using the GPIO_Input_Init function
void switch_Init(void)
{
  GPIO_Input_Init(SW0_port, SW0_pin, ENABLE);
  GPIO_Input_Init(SW1_port, SW1_pin, ENABLE);
  GPIO_Input_Init(SW2_port, SW2_pin, ENABLE);
  GPIO_Input_Init(SW3_port, SW3_pin, ENABLE);
}

// Reads the current (parameterized) switch, includes a debounce within the conditional statement
uint8_t switch_Read(volatile GPIO_port_t *port_addr, uint8_t pin_mask)
{
    uint8_t val = 1;
    val = GPIO_Input_Read(port_addr, pin_mask);

    if(val == PRESSED)
    {
        _delay_ms(75);
        val = GPIO_Input_Read(port_addr, pin_mask);        
    }
    
    return val;
}