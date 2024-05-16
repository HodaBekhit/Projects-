#include "MCAL/SYSTICK/STK.h"
#include "MCAL/RCC/RCC.h"
#include "HAL/LED/LED.h"
#include "HAL/LED/LED_cfg.h"

static void ToggleLed(void);
/*int main(void)
{
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
	LED_Init();
	STK_Init();
	STK_SetTime_ms(1000);
	STK_RegisterCallback(ToggleLed);
	STK_Start(STK_Periodic);
	while(1);
}
void ToggleLed(void)
{
	static u32 status = LED_STATE_OFF; // Initial state set to LED_STATE_OFF
	// Toggle LED status between LED_STATE_OFF and LED_STATE_ON
	status ^= (LED_STATE_ON | LED_STATE_OFF);
	LED_SetStatus(LED_RED, status);
}*/
