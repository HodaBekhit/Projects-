
#include "MCAL/GPIO/GPIO.h"
#include "LED.h"

/*** LEDS user configuration ***/
const LED_cfg_t LEDS[_LED_NUM]={
	[LED_RED]={.LED_Port=GPIO_A, .LED_Pin=GPIO_PIN3, .LED_Conn=LED_CONN_FWD, .LED_Mode=output_PP, .LED_Speed=High_speed},
	[LED_GREEN]={.LED_Port=GPIO_A, .LED_Pin=GPIO_PIN4, .LED_Conn=LED_CONN_FWD, .LED_Mode=output_PP, .LED_Speed=High_speed}
   // ,[LED_YELLOW]={.LED_Port=GPIO_A, .LED_Pin=GPIO_PIN2, .LED_Conn=LED_CONN_FWD, .LED_Mode=output_PP, .LED_Speed=High_speed}
};
