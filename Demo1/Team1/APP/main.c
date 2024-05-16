/******************************************************************************************/
/*                                       Includes                                         */
/******************************************************************************************/
#include "MCAL/RCC/RCC.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/NVIC/NVIC.h"
#include "MCAL/NVIC/stm32f401cc.h"
#include "MCAL/USART/USART.h"
#include "HAL/LED/LED.h"
u8 buffer[3]={0};
u8 buffer2[3]={'y','e','s'};
/*Test CBF*/
void CBF(void)
{
    if(buffer[0]=='y'&&buffer[1]=='e'&&buffer[2]=='s')
    {
         USART_TxBufferAsyncZeroCopy(USART_CH1,buffer,3,NULLPTR);
    }
}

/*int main (void)
{
    u8 ch=0;
	GPIO_PinCfg_t GPIO_Tx1Pin={ .GPIO_PORT=GPIO_B , .GPIO_PIN= GPIO_PIN6,  .GPIO_MODE=AF_PP , .GPIO_SPEED= High_speed, .GPIO_AF=AF7};
	GPIO_PinCfg_t GPIO_Rx1Pin={ .GPIO_PORT=GPIO_B, .GPIO_PIN= GPIO_PIN7, .GPIO_MODE=AF_PP , .GPIO_SPEED=High_speed , .GPIO_AF=AF7};

	// enable GPIOB periph clock for led and UART TX & RX
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOBEN,Periph_enuON);
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
	// enable the usart1 periph clock
    RCC_enuEnableDisablePeripheral(RCC_APB2, USART1EN, Periph_enuON);
    //NVIC Init
     NVIC_EnableInterrupt(NVIC_USART1_INTERRUPT);
	//init uart pins
	GPIO_InitPin(&GPIO_Tx1Pin);
    GPIO_InitPin(&GPIO_Rx1Pin);

	LED_Init();


    
    //NVIC_SetPending(NVIC_USART1_INTERRUPT);

   
    USART_PostCompileCfg_t USART1_cfg={USART_CH1,USART_OVERSAMPLING_16,USART_DATA_BITS_8,USART_PARITY_NONE,USART_STOP_BITS_ONE,9600};
    USART_Init(&USART1_cfg);

   // USART_RxBufferAsyncZeroCopy(USART_CH1,buffer,3,CBF);
    //USART_TxBufferAsyncZeroCopy(USART_CH1,buffer2,3,NULLPTR);

    while(1)
    {
USART_RxBufferAsyncZeroCopy(USART_CH1,buffer,3,CBF);
        // USART_GetByte(USART_CH1,&ch);
        // if(ch=='Y')
        // {
        //     LED_SetStatus(LED_RED,LED_ON);
        // }
        // else
        // {
        //     LED_SetStatus(LED_RED,LED_OFF);
        // }
        // USART_SendByte(USART_CH1,ch);
    }
}

*/
