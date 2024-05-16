#ifndef MCAL_GPIO_GPIO_H_
#define MCAL_GPIO_GPIO_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_LIB/std_types.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/*** GPIO_Ports Base addresses ***/
#define GPIO_A ((void*)0x40020000)
#define GPIO_B ((void*)0x40020400)
#define GPIO_C ((void*)0x40020800)

/*** GPIO Pins index ***/
#define GPIO_PIN0    0x00000000
#define GPIO_PIN1    0x00000001
#define GPIO_PIN2    0x00000002
#define GPIO_PIN3    0x00000003
#define GPIO_PIN4    0x00000004
#define GPIO_PIN5    0x00000005
#define GPIO_PIN6    0x00000006
#define GPIO_PIN7    0x00000007
#define GPIO_PIN8    0x00000008
#define GPIO_PIN9    0x00000009
#define GPIO_PIN10   0x0000000A
#define GPIO_PIN11   0x0000000B
#define GPIO_PIN12   0x0000000C
#define GPIO_PIN13   0x0000000D
#define GPIO_PIN14   0x0000000E
#define GPIO_PIN15   0x0000000F

/*** GPIO Modes ***/
#define output_PP    0b01000
#define output_PP_PU 0b01001
#define output_PP_PD 0b01010
#define output_OD    0b01100
#define output_OD_PU 0b01101
#define output_OD_PD 0b01110
#define input_floating 0b00000
#define input_PU     0b00001
#define input_PD     0b00010
#define AF_PP        0b10000
#define AF_PP_PU     0b10001
#define AF_PP_PD     0b10010
#define AF_OD        0b10100
#define AF_OD_PU     0b10101
#define AF_OD_PD     0b10110
#define analog       0b11000

/*** GPIO Speed ***/
#define Low_speed      0b00
#define Medium_speed   0b01
#define High_speed     0b10
#define VeryHigh_speed 0b11

/*** GPIO AF ***/
#define AF0 0b0000
#define AF1 0b0001
#define AF2 0b0010
#define AF3 0b0011
#define AF4 0b0100
#define AF5 0b0101
#define AF6 0b0110
#define AF7 0b0111
#define AF8 0b1000
#define AF9 0b1001
#define AF10 0b1010
#define AF11 0b1011
#define AF12 0b1100
#define AF13 0b1101
#define AF14 0b1110
#define AF15 0b1111

/*** GPIO_Value ***/
#define GPIO_LOW  0x00010000
#define GPIO_HIGH 0x00000001

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
/*** GPIO Registers ***/
typedef struct{
	volatile u32 MODER;
	volatile u32 OTYPER;
	volatile u32 OSPEEDR;
	volatile u32 PUPDR;
	volatile u32 IDR;
	volatile u32 ODR;
	volatile u32 BSRR;
	volatile u32 LCKR;
	volatile u64 AFR; 
	/* volatile u32 AFRL;
	volatile u32 AFRH;*/
	}GPIO_t;
	
/*** GPIO Configurations ***/	
typedef struct {
	void* GPIO_PORT; 
	u32 GPIO_PIN; 
	u32 GPIO_MODE; 
	u32 GPIO_SPEED;
	u32 GPIO_AF;
	}GPIO_PinCfg_t; 
	
/*** GPIO Errors ***/ 	
typedef enum
{
	GPIO_OK,
	GPIO_InvalidPort,
	GPIO_InvalidPin,
	GPIO_Invalidspeed,
	GPIO_Invalidmode,
	GPIO_NULLPTR,
	GPIO_Invalidvalue
	}GPIO_ErrorStatus_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
/****************************** GPIO_InitPin *****************************************
* @description  :  Initializes a GPIO pin according to the provided configuration.
*
* @param        :  AddPinCfg - Pointer to a structure containing pin configuration:
*                                - GPIO_PORT: Pointer to the GPIO port (GPIO_A/B/C)
*                                - GPIO_PIN: Pin index (GPIO_PIN0 ~ GPIO_PIN15)
*                                - GPIO_MODE: Pin mode:
*                                             - output_PP, output_PP_PU, output_PP_PD,
*                                             - output_OD, output_OD_PU, output_OD_PD,
*                                             - input_floating, input_PU, input_PD,
*                                             - AF_PP, AF_PP_PU, AF_PP_PD,
*                                             - AF_OD, AF_OD_PU, AF_OD_PD,
*                                             - analog
*                                - GPIO_SPEED: Pin speed:
*                                              - Low_speed
*                                              - Medium_speed
*											   - High_speed
*											   - VeryHigh_speed
*                                - GPIO_AF: Pin alternate function (e.g., AF0 ~ AF15)
* @return       :  GPIO_ErrorStatus_t:
*								- GPIO_OK
*								- GPIO_InvalidPort
*								- GPIO_InvalidPin
*								- GPIO_Invalidspeed
*								- GPIO_Invalidmode
*								- GPIO_NULLPTR
****************************************************************************************/
GPIO_ErrorStatus_t GPIO_InitPin(GPIO_PinCfg_t* AddPinCfg);

/****************************** GPIO_SetPinValue *****************************************
* @description :Sets the value of a specific pin on a GPIO port to(GPIO_HIGH or GPIO_LOW)*                    
*																						 *
* @param        :  Copy_Port - Pointer to the GPIO port (GPIO_A/B/C)                *
*                  Copy_Pin  - Pin index (GPIO_PIN0 ~ GPIO_PIN15)                        *
*                  Copy_Value - Value to be set for the pin (GPIO_HIGH or GPIO_LOW)      *       
*                                                                                        *
* @return       :  GPIO_ErrorStatus_t:
*								- GPIO_OK
*								- GPIO_InvalidPort
*								- GPIO_InvalidPin
*								- GPIO_Invalidvalue
*								- GPIO_NULLPTR                           
****************************************************************************************/
GPIO_ErrorStatus_t GPIO_SetPinValue(void* Copy_Port,u32 Copy_Pin,u32 Copy_Value);

/****************************** GPIO_GetPinValue *****************************************
* @description  :  Reads the value of a specific pin on a GPIO port.                     *
*																						 *
* @param        :  Copy_Port - Pointer to the GPIO port (GPIO_A/B/C)               *
*                  Copy_Pin  - Pin index (GPIO_PIN0 ~ GPIO_PIN15)                        *
*                  Add_Value - Pointer to where the pin value will be stored             *
*                                                                                        *
** @return       :  GPIO_ErrorStatus_t:													 * 
*								- GPIO_OK                                                *
*								- GPIO_InvalidPort
*								- GPIO_InvalidPin
*								- GPIO_NULLPTR                                          *
****************************************************************************************/
GPIO_ErrorStatus_t GPIO_GetPinValue(void* Copy_Port,u32 Copy_Pin,u8* Add_Value);




#endif // MCAL_GPIO_GPIO_H_