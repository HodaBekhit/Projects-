/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "GPIO.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/*** masks ***/
#define MODER_Mask   0b11000
#define OTYPER_Mask  0b00100
#define PUPDR_Mask   0b00011
#define Clear2bits   0x00000003
#define Clear1bit    0x00000001
#define _4_BIT_MASK  0x0000000F
#define PIN_OFFSET_4 0x00000004
#define PIN_ALTERNATIVE_OFFSET 0x00000004

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

GPIO_ErrorStatus_t GPIO_InitPin(GPIO_PinCfg_t* AddPinCfg)
{
	GPIO_ErrorStatus_t GPIO_RetError= GPIO_OK;
	if (AddPinCfg->GPIO_PORT != GPIO_A &&
    AddPinCfg->GPIO_PORT != GPIO_B &&
    AddPinCfg->GPIO_PORT != GPIO_C) {
    GPIO_RetError= GPIO_InvalidPort;
}
  else if(AddPinCfg->GPIO_PIN > GPIO_PIN15)
  {
	 GPIO_RetError= GPIO_InvalidPin;
  }
  else if (AddPinCfg->GPIO_SPEED>VeryHigh_speed)
  {
	   GPIO_RetError= GPIO_Invalidspeed;
  }
  else if(!(AddPinCfg->GPIO_MODE == output_PP ||
  AddPinCfg->GPIO_MODE == output_PP_PU ||
  AddPinCfg->GPIO_MODE == output_PP_PD ||
  AddPinCfg->GPIO_MODE == output_OD ||
  AddPinCfg->GPIO_MODE == output_OD_PU ||
  AddPinCfg->GPIO_MODE == output_OD_PD ||
  AddPinCfg->GPIO_MODE == input_floating ||
  AddPinCfg->GPIO_MODE == input_PU ||
  AddPinCfg->GPIO_MODE == input_PD ||
  AddPinCfg->GPIO_MODE == AF_PP ||
  AddPinCfg->GPIO_MODE == AF_PP_PU ||
  AddPinCfg->GPIO_MODE == AF_PP_PD ||
  AddPinCfg->GPIO_MODE == AF_OD ||
  AddPinCfg->GPIO_MODE == AF_OD_PU ||
  AddPinCfg->GPIO_MODE == AF_OD_PD ||
  AddPinCfg->GPIO_MODE == analog)) {
	  GPIO_RetError= GPIO_Invalidmode;
  }
  else if (AddPinCfg==NULLPTR)
  {
	   GPIO_RetError= GPIO_NULLPTR;
  }

 else
 {
	u32 LocTempRegister;
	/* set the mode */
	LocTempRegister=((GPIO_t*)(AddPinCfg->GPIO_PORT))->MODER;
	LocTempRegister&=~(Clear2bits<<(AddPinCfg->GPIO_PIN*2));
	LocTempRegister|=(((AddPinCfg->GPIO_MODE&MODER_Mask)>>3)<<(AddPinCfg->GPIO_PIN*2));
	((GPIO_t*)(AddPinCfg->GPIO_PORT))->MODER=LocTempRegister;
	
	/* set the output type */
	LocTempRegister=((GPIO_t*)(AddPinCfg->GPIO_PORT))->OTYPER;
	LocTempRegister&=~(Clear1bit<<(AddPinCfg->GPIO_PIN));
	LocTempRegister|=(((AddPinCfg->GPIO_MODE&OTYPER_Mask)>>2)<<(AddPinCfg->GPIO_PIN));
	((GPIO_t*)(AddPinCfg->GPIO_PORT))->OTYPER=LocTempRegister;
	
	/* set the PUPDR */
	LocTempRegister=((GPIO_t*)(AddPinCfg->GPIO_PORT))->PUPDR;
	LocTempRegister&=~(Clear2bits<<(AddPinCfg->GPIO_PIN*2));
	LocTempRegister|=((AddPinCfg->GPIO_MODE&PUPDR_Mask)<<(AddPinCfg->GPIO_PIN*2));
	((GPIO_t*)(AddPinCfg->GPIO_PORT))->PUPDR=LocTempRegister;
	
	/* set the speed */
	LocTempRegister=((GPIO_t*)(AddPinCfg->GPIO_PORT))->OSPEEDR;
	LocTempRegister&=~(Clear2bits<<(AddPinCfg->GPIO_PIN*2));
	LocTempRegister|=(AddPinCfg->GPIO_SPEED <<(AddPinCfg->GPIO_PIN*2));
	((GPIO_t*)(AddPinCfg->GPIO_PORT))->OSPEEDR=LocTempRegister;

	/* Set the AF */
	LocTempRegister=((GPIO_t*)(AddPinCfg->GPIO_PORT))->AFR;
	LocTempRegister&= ~( ((u64)_4_BIT_MASK) << (AddPinCfg->GPIO_PIN*PIN_OFFSET_4) );
	LocTempRegister|=(((u64)AddPinCfg->GPIO_AF) << (AddPinCfg->GPIO_PIN*PIN_OFFSET_4) );
	((GPIO_t*)(AddPinCfg->GPIO_PORT))->AFR = LocTempRegister;
			/* AF */
		/*if(AddPinCfg->GPIO_PIN <= GPIO_PIN7){
			((GPIO_t*)(AddPinCfg->GPIO_PORT))->AFRL |= AddPinCfg->GPIO_AF << (PIN_ALTERNATIVE_OFFSET * (AddPinCfg->GPIO_PIN%8));
		}
		else{
			((GPIO_t*)(AddPinCfg->GPIO_PORT))->AFRH |= AddPinCfg->GPIO_AF << (PIN_ALTERNATIVE_OFFSET * (AddPinCfg->GPIO_PIN%8));
		}*/


 }
	return GPIO_RetError;
}
GPIO_ErrorStatus_t GPIO_SetPinValue(void* Copy_Port,u32 Copy_Pin,u32 Copy_Value)
{
	GPIO_ErrorStatus_t GPIO_RetError= GPIO_OK;
    if( Copy_Pin > GPIO_PIN15)
	{
		GPIO_RetError= GPIO_InvalidPin;
	}
	else if (!(Copy_Value==GPIO_LOW || Copy_Value==GPIO_HIGH))
	{
		GPIO_RetError= GPIO_Invalidvalue;
	}
	else if (Copy_Port==NULLPTR)
	{
		GPIO_RetError=GPIO_NULLPTR;
	}
	else
	{
		((GPIO_t*)Copy_Port)->BSRR =Copy_Value<<Copy_Pin;
	}
	return GPIO_RetError;
}
GPIO_ErrorStatus_t GPIO_GetPinValue(void* Copy_Port,u32 Copy_Pin,u8* Add_Value)
{
	GPIO_ErrorStatus_t GPIO_RetError= GPIO_OK;
	if( Copy_Pin > GPIO_PIN15)
	{
		GPIO_RetError= GPIO_InvalidPin;
	}
	else if (Copy_Port==NULLPTR || Add_Value==NULLPTR )
	{
		GPIO_RetError=GPIO_NULLPTR;
	}
	else
	{
	*Add_Value = (((GPIO_t*)Copy_Port)->IDR >> Copy_Pin)&0x01;
	}
	
	return GPIO_RetError;
}

