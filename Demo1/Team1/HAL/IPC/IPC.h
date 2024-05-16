#include "MCAL/RCC/RCC.h"
#include "MCAL/GPIO/GPIO.h"
#include "MCAL/USART/USART.h"
#include "MCAL/NVIC/NVIC.h"
#include "STD_LIB/std_types.h"

#define USART1_TxPORT        GPIO_B
#define USART1_RxPORT        GPIO_B
#define USART1_TxPIN         GPIO_PIN6
#define USART1_RxPIN         GPIO_PIN7


void IPC_Init(u8 UART_chanel);

void IPC_SendUSART(u8 USART_Num,u8* buffer, u16 len, fnpointer cbf);

void IPC_ReceiveUSART(u8 USART_Num,u8*buffer, u16 len, fnpointer cbf);
