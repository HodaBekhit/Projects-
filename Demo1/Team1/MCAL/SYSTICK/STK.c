/*
 * STK.c
 *
 *  Created on: Mar 9, 2024
 *      Author: Dell
 */

#include "STK.h"
#include "STK_cfg.h"

#define Enable_Mask 0x00000001

#define Max_ReloadValue 0x00ffffff


/*SYSTICK Base Address*/
#define STK_BASEADDRESS (0xE000E010UL)
/* the base address as pointer to the SYSTICK registers*/
#define STK ((void* )STK_BASEADDRESS)

/* Static Pointer systickcbf_t type, Points to APP_cbf*/
static STK_cbf_t APP_cbf=NULLPTR;

static u8 STK_global_Mode=STK_OneTime;

/* SYSTICK Registers definitions */
typedef struct
{
	volatile u32 CTRL;
    volatile u32 LOAD;
    volatile u32 VAL;
    volatile u32 CALIB;
 }STK_RegisterOffset_t;

 STK_ErrorStatus_t  STK_Init(void)
 {
	 STK_ErrorStatus_t STK_RetErrorStatus = STK_OK;
	 if(!((STK_CLKSOURCE==AHB_div_8)||(STK_CLKSOURCE==Proc_clk_AHB)))
	 {
		 STK_RetErrorStatus = STK_NOK;
	 }
	 else
	 {
		 /* Set Clock Source */
		 ((STK_RegisterOffset_t*)(STK))->CTRL|=STK_CLKSOURCE;
		 /* Set Interrupt mode */
		 ((STK_RegisterOffset_t*)(STK))->CTRL|=STK_TICKINT;
	 }

	 return STK_RetErrorStatus;
 }

 void STK_Start(u8 STK_Mode)
 {
	 /* Set The timer mode */
	 STK_global_Mode=STK_Mode;

	 /* clear the current timer value */
	 ((STK_RegisterOffset_t*)(STK))->VAL = 1U;

    /* enable counter*/
    ((STK_RegisterOffset_t*)(STK))->CTRL|=Enable_Mask;
 }
 void STK_Stop(void)
 {
    /* disable counter*/
	((STK_RegisterOffset_t*)(STK))->CTRL&=~(Enable_Mask);

 }

 STK_ErrorStatus_t STK_SetTime_ms(u32 CopyTime_ms)
 {
     STK_ErrorStatus_t STK_RetErrorStatus = STK_OK;

     /* Determine the STK clock source frequency based on the selected clock source */
     u32 STK_CLKSRC_FREQ = (STK_CLKSOURCE == Proc_clk_AHB) ? STK_AHB_FREQ : (STK_AHB_FREQ / 8);

    /* Set reload value in STK_LOAD Register */
    u32 Reload_Value = ((CopyTime_ms * STK_CLKSRC_FREQ) / 1000) - 1;

    if(Reload_Value>Max_ReloadValue)
    {
    	STK_RetErrorStatus=STK_NOK;
    }
    else
    {
    	((STK_RegisterOffset_t*)STK)->LOAD=Reload_Value;
    }
     return STK_RetErrorStatus;
 }


 /*STK_ErrorStatus_t STK_IsExpired(void)
 {

 }
*/
 STK_ErrorStatus_t STK_RegisterCallback(STK_cbf_t cbf)
 {
     STK_ErrorStatus_t STK_RetErrorStatus = STK_OK;
     if(cbf)
     {
    	 APP_cbf=cbf;
     }
     else
     {
    	 STK_RetErrorStatus=STK_NOK;
     }

     return STK_RetErrorStatus;
 }

 void SysTick_Handler(void)
 {
	 if(APP_cbf)
	 {
		 APP_cbf();
		 if(STK_global_Mode==STK_OneTime)
		 {

			 STK_Stop();
		 }
		 else
		 {
			/* do nothing*/
		 }
	 }
	 else
	 {
		 /*do nothing*/
	 }
 }
