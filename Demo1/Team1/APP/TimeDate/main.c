#include "APP/APP_Demo1/APP_Demo.h"


int main(void)
{
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
	IPC_Init(USART_CH1);
	SWITCH_Init();
	LCD_InitPins();
	LCD_init_asynch(NULLPTR);
	scheduler_init();
	APP_LCDMainMenu();
	scheduler_start();
}


