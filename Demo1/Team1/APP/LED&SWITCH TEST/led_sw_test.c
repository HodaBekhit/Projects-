/**
 ******************************************************************************
 * @file           : main.c
 * @author         : Hoda Ahmed
 * @brief          : Main program body
 ******************************************************************************/

#include "MCAL/RCC/RCC.h"
#include "MCAL/GPIO/GPIO.h"
#include "HAL/LED/LED.h"
#include "HAL/SWITCH/SWITCH.h"

/*int main(void)
{
	u8 sw_state = SWITCH_NOT_PRESSED;

	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOBEN,Periph_enuON);
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);

	 LED_Init();
	 SWITCH_Init();
   while(1)
   {
	SWITCH_GetStatus(SWITCH_ONE, &sw_state);
	if(sw_state==SWITCH_PRESSED)
	{
		LED_SetStatus(LED_RED, LED_STATE_ON);
		LED_SetStatus(LED_GREEN, LED_STATE_OFF);
	}
	else
	{
		LED_SetStatus(LED_RED, LED_STATE_OFF);
		LED_SetStatus(LED_GREEN, LED_STATE_ON);
	}

   }
}*/
