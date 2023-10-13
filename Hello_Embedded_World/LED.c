#include "LED.h"

unsigned char LED_State_g = 0;

//Initializes the LED by calling the GPIO init function (this sets DDR to 1)
void LEDS_Init(volatile GPIO_port_t * Port_addr, uint8_t LED_pin)
{
  GPIO_Output_Init(Port_addr,LED_pin); // Also set an initial state
}

// Turns the LED off by calling the GPIO_output_set function.
void LEDS_Off(volatile GPIO_port_t * Port_addr, uint8_t LED_pin)
{
  GPIO_Output_Set(Port_addr,LED_pin); //func calls func to execute
}

// Turns the LED off by calling the GPIO_output_set function.
void LEDS_On(volatile GPIO_port_t * Port_addr, uint8_t LED_pin)
{
  GPIO_Output_Clear(Port_addr,LED_pin);
}
// With inline function, func called to execute

// Initializes all LEDs and start them in the off position in case they weren't
void LED_Flash_Init(void)
{
	GPIO_Output_Init(LED0_port, LED0_pin);
	GPIO_Output_Init(LED1_port, LED1_pin); 
	GPIO_Output_Init(LED2_port, LED2_pin); 
	GPIO_Output_Init(LED3_port, LED3_pin);

	LEDS_Off(LED0_port, LED0_pin);
	LEDS_Off(LED1_port, LED1_pin);
	LEDS_Off(LED2_port, LED2_pin);
	LEDS_Off(LED3_port, LED3_pin);
}

// Changes all LEDs at once to the opposite status
void LED_Flash_Change_State(void)
{
	if(LED_State_g==0)
	{
		GPIO_Output_Set(LED0_port, LED0_pin);
		GPIO_Output_Set(LED1_port, LED1_pin);
		GPIO_Output_Set(LED2_port, LED2_pin);
		GPIO_Output_Set(LED3_port, LED3_pin);
		LED_State_g=1;
	}
	else
	{
		GPIO_Output_Clear(LED0_port, LED0_pin);
		GPIO_Output_Clear(LED1_port, LED1_pin);
		GPIO_Output_Clear(LED2_port, LED2_pin);
		GPIO_Output_Clear(LED3_port, LED3_pin);
		LED_State_g=0;
	}
}