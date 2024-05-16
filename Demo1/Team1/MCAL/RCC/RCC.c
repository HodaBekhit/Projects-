/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/

#include "RCC.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/*RCC Base Address*/
#define RCC_BASEADDRESS 0x40023800
/* the base address as pointer to the RCC registers*/
#define RCC ((Rcc_RegisterOffset_t* )RCC_BASEADDRESS)
/******** SW values ******/
#define HSI_SW 0
#define HSE_SW 1
#define PLL_SW 2
/******** SWS values *******/
#define HSI_SWS 0x00000000
#define HSE_SWS 0x00000004
#define PLL_SWS 0x00000008
/********* masked bits ********/
#define HSIEN_MASK 0x00000001
#define HSEEN_MASK 0x00010000
#define PLLEN_MASK 0x01000000
#define HSIRDY_MASK 0x00000002
#define HSERDY_MASK 0x00020000
#define PLLRDY_MASK 0x02000000
#define SWS_MASK  0x0000000C
#define SW_MASK   0x00000003
#define PLL_MASK  0x0f437fff
#define AHBPRE_MASK 0x000000f0
#define APB1PRE_MASK 0x00001c00
#define APB2PRE_MASK 0x0000e000

/********* RCC PLL configuration register (RCC_PLLCFGR) PINS *********/
#define PLLM0 0
#define PLLN0 6
#define PLLP0 16
#define PLLSRC 22
#define PLLQ0 24

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/*** RCC Registers offset***/
typedef struct
{
	volatile u32 CR;
	volatile u32 PLLCFGR;
	volatile u32 CFGR;
	volatile u32 CIR;
	volatile u32 AHB1RSTR;
	volatile u32 AHB2RSTR;
	volatile u32 Reserved[2];
	volatile u32 APB1RSTR;
	volatile u32 APB2RSTR;
	volatile u32 Reserved1[2];
	volatile u32 AHB1ENR;
	volatile u32 AHB2ENR;
	volatile u32 Reserved2[2];
	volatile u32 APB1ENR;
	volatile u32 APB2ENR;
	volatile u32 Reserved3[2];
	volatile u32 AHB1LPENR;
	volatile u32 AHB2LPENR;
	volatile u32 Reserved4[2];
	volatile u32 APB1LPENR;
	volatile u32 APB2LPENR;
	volatile u32 Reserved5[2];
	volatile u32 BDCR;
	volatile u32 CSR;
	volatile u32 Reserved6[2];
	volatile u32 SSCGR;
	volatile u32 PLLI2SCFGR;
	volatile u32 Reserved7;
	volatile u32 DCKCFGR;
}Rcc_RegisterOffset_t;

/*** PLL Config. ***/
RCC_strPLLConfig_t PLLstr_Config={
	/*RCC_enuPLLClockSource: 0->HSI clock selected, 1->HSE oscillator clock*/
	.RCC_enuPLLClockSource=0 ,
	/*RCC_enuPLL_M:2~63*/
	.RCC_enuPLL_M= 2,
	/*RCC_enuPLL_N:192~432*/
	.RCC_enuPLL_N= 192,
	/*RCC_enuPLL_P: 2/4/6/8*/
	.RCC_enuPLL_P=2 ,
	/*RCC_enuPLL_Q: 2~15*/
	.RCC_enuPLL_Q=2
};

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/


/**
 *@brief : Function to control the clock state ON/OFF 
 *@param : Copy_u8ClockSource: RCC_HSE/RCC_HSI/RCC_PLL
           Copy_enuClockState: CLK_ON/CLK_OFF
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
          -Controlling the sys_clk
		  -Choosing wrong clock
	      -Choosing wrong clock state                                                                  
 */
RCC_enuErrorStatus_t RCC_enuControlClock(u8 Copy_u8ClockSource, RCC_enuClkStatus_t Copy_enuClockState )
{
	/* GET the SWS bits value*/
	 u32 Loc_u32SystemCLOCK = ((RCC->CFGR & SWS_MASK)<<2);
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	
	/* ERROR :the clock source used as the system clock, cannot be directly turned on or off during runtime.*/
	if(Loc_u32SystemCLOCK==Copy_u8ClockSource)
	{
		RCC_enuErrorState=RCC_enuINVALID_CLKCTRL;
	}
	else if((Copy_u8ClockSource<RCC_HSI) || (Copy_u8ClockSource>RCC_PLL))
	{
		RCC_enuErrorState=RCC_enuINVALID_CLKSRC;
	}
	else if((Copy_enuClockState<CLK_OFF) || (Copy_enuClockState>CLK_ON))
	{
		RCC_enuErrorState=RCC_enuINVALID_CLKSTATE;
	}
	else
	{
		switch (Copy_u8ClockSource)
		{
			/* Enable/Disable HSI */
			case RCC_HSI:
			switch(Copy_enuClockState)
			{
				/*HSI OFF */
				case CLK_OFF:
				RCC->CR &=~(HSIEN_MASK);
				break;
				
				/* HSI ON*/
				case CLK_ON:
				RCC->CR |=HSIEN_MASK;
				/* wait till HSI ready,
				 we should set time out value*/
				while(!(RCC->CR & HSIRDY_MASK));
				break;
			}
			break;
			
			/* Enable/Disable HSE */
			case RCC_HSE:
			switch(Copy_enuClockState)
			{
				/*HSE OFF */
				case CLK_OFF:
				RCC->CR &=~(HSEEN_MASK);
				break;
				
				/*HSE ON */
				case CLK_ON:
				RCC->CR |=HSEEN_MASK;
				/* wait till HSI ready,
				 we should set time out value*/
				while(!(RCC->CR & HSERDY_MASK));
				break;
			}
			break;
			
			/* Enable/Disable PLL */
			case RCC_PLL:
			switch(Copy_enuClockState)
			{
				/*PLL OFF */
				case CLK_OFF:
				RCC->CR &=~(PLLEN_MASK);
				break;
				
				/*PLL ON */
				case CLK_ON:
				RCC->CR |=PLLEN_MASK;
				/* wait till HSI ready,
				 we should set time out value*/
				while(!(RCC->CR & PLLRDY_MASK));
				break;
			}
			break;	
		}
	}
	return RCC_enuErrorState;
}

/**
 *@brief : Function to select the system clock 
 *@param : Copy_u8ClockSource: RCC_HSE/RCC_HSI/RCC_PLL
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
		  -Choosing wrong clock
	      -Choosing wrong clock state  
		  -Clock not ready
		  -Clock not enabled
		  -Wrong SWS                                                                   
 */
RCC_enuErrorStatus_t RCC_enuSelectClock(u8 Copy_u8ClockSource)
{
	u32 Loc_u32RCC_CFGR;
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	
	/** error handling ***/
	 if((Copy_u8ClockSource<RCC_HSI) || (Copy_u8ClockSource>RCC_PLL))
	 {
		 RCC_enuErrorState=RCC_enuINVALID_CLKSRC;
	 }
	 else
	 {
		 switch(Copy_u8ClockSource)
		 {
			 /* select HSI as SYS_CLK */
			 case RCC_HSI:
			 /*Check if HSI is ON*/
			 if(RCC->CR & HSIEN_MASK)
			 {
				 /*Check HSI ready flag */
				 if(RCC->CR & HSIRDY_MASK)
				 {
					 /* Select sys_clk: set SW*/
					 Loc_u32RCC_CFGR  = RCC->CFGR;
					 // Clear SW0 and SW1 bits within RCC_CFGR
					 Loc_u32RCC_CFGR &= ~(SW_MASK);
					 // Set desired value to SW0 and SW1
					 Loc_u32RCC_CFGR |= HSI_SW;
					 // Update RCC_CFGR register with the modified value
					 RCC->CFGR = Loc_u32RCC_CFGR; 
					 
					 /* Check that the CLK selected is actual CLK working (SWS=SW),
					 should i add time out? */
					 if((RCC->CFGR & SWS_MASK) == HSI_SWS)
					 {
						 /* do nothing */
					 }
					 else
					 {
						 RCC_enuErrorState=RCC_enuSWS_NOTMATCH;
					 }
				 }
				 /* ERROR: CLK not ready*/
				 else
				 {
					 RCC_enuErrorState=RCC_enuNOTRDY;
				 } 
			 }
			 /*ERROR: CLK not enable*/
			 else
			 {
				 RCC_enuErrorState=RCC_enuCLKOFF;
			 }
			 break;
			 
			/* select HSE as SYS_CLK */
			 case RCC_HSE:
			 /*Check if HSE is ON*/
			 if(RCC->CR & HSEEN_MASK)
			 {
				 /*Check HSE ready flag */
				 if(RCC->CR & HSERDY_MASK)
				 {
					 /* Select sys_clk: set SW*/
					 Loc_u32RCC_CFGR  = RCC->CFGR;
					 // Clear SW0 and SW1 bits within RCC_CFGR
					 Loc_u32RCC_CFGR &= ~(SW_MASK);
					 // Set desired value to SW0 and SW1
					 Loc_u32RCC_CFGR |= HSE_SW;
					 // Update RCC_CFGR register with the modified value
					 RCC->CFGR = Loc_u32RCC_CFGR; 
					 
					 /* Check that the CLK selected is actual CLK working (SWS=SW),
					 should i add time out? */
					 if((RCC->CFGR & SWS_MASK) == HSE_SWS)
					 {
						 /* do nothing */
					 }
					 else
					 {
						 RCC_enuErrorState=RCC_enuSWS_NOTMATCH;
					 }
					  
				 }
				 /* ERROR: CLK not ready*/
				 else
				 {
					 RCC_enuErrorState=RCC_enuNOTRDY;
				 } 
			 }
			 /*ERROR: CLK not enable*/
			 else
			 {
				 RCC_enuErrorState=RCC_enuCLKOFF;
			 }
			 break;
			 
			/* select PLL as SYS_CLK */
			 case RCC_PLL:
			 /*Check if PLL is ON*/
			 if(RCC->CR & PLLEN_MASK)
			 {
				 /*Check PLL ready flag */
				 if(RCC->CR & PLLRDY_MASK)
				 {
					 /* Select sys_clk: set SW*/
					 Loc_u32RCC_CFGR  = RCC->CFGR;
					 // Clear SW0 and SW1 bits within RCC_CFGR
					 Loc_u32RCC_CFGR &= ~(SW_MASK);
					 // Set desired value to SW0 and SW1
					 Loc_u32RCC_CFGR |= PLL_SW;
					 // Update RCC_CFGR register with the modified value
					 RCC->CFGR = Loc_u32RCC_CFGR; 
					 
					 /* Check that the CLK selected is actual CLK working (SWS=SW),
					 should i add time out? */
					 if((RCC->CFGR & SWS_MASK) == PLL_SWS)
					 {
						 /* do nothing */
					 }
					 else
					 {
						 RCC_enuErrorState=RCC_enuSWS_NOTMATCH;
					 }
					  
				 }
				 /* ERROR: CLK not ready*/
				 else
				 {
					 RCC_enuErrorState=RCC_enuNOTRDY;
				 } 
			 }
			 /*ERROR: CLK not enable*/
			 else
			 {
				 RCC_enuErrorState=RCC_enuCLKOFF;
			 }
			 break;
			 
	 }
}
 return RCC_enuErrorState;
}

/**
 *@brief : Function to configure PLL clock source  
 *@param : ADD_strPLLConfig: pointer to struct contains
           PLL configuration parameters( CLK_SRC/M/N/P/Q)
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
                                                                           
 */
RCC_enuErrorStatus_t RCC_enuConfigPLL(RCC_strPLLConfig_t* ADD_strPLLConfig)
{
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	u32  Loc_u32PLL_M=ADD_strPLLConfig->RCC_enuPLL_M;
	u32 Loc_u32PLL_N=ADD_strPLLConfig->RCC_enuPLL_N;
	u32  Loc_u32PLL_P=ADD_strPLLConfig->RCC_enuPLL_P;
	u32  Loc_u32PLL_Q=ADD_strPLLConfig->RCC_enuPLL_Q;
	u32  Loc_u32PLL_CLKSRC=ADD_strPLLConfig->RCC_enuPLLClockSource;
	
	/* Checking errors */
	
	/* Invalid M config. */
	if(Loc_u32PLL_M<2 || Loc_u32PLL_M>63)
	{
		RCC_enuErrorState=RCC_enuINVALID_M;
	}
	/* Invalid N config. */
	else if(Loc_u32PLL_N<192 || Loc_u32PLL_N>432)
	{
		RCC_enuErrorState=RCC_enuINVALID_N;
	}
	/* Invalid P config. */
	else if (!(Loc_u32PLL_P == 2 || Loc_u32PLL_P == 4 || Loc_u32PLL_P == 6 || Loc_u32PLL_P == 8))
	{
		RCC_enuErrorState=RCC_enuINVALID_P;
	}
	/* Invalid Q config. */
	else if(Loc_u32PLL_Q<2 || Loc_u32PLL_Q>15)
	{
		RCC_enuErrorState=RCC_enuINVALID_Q;
	}
	/* Config. PLL ON */
	else if(!(Loc_u32PLL_CLKSRC==1|| Loc_u32PLL_CLKSRC==0 ))
	{
		RCC_enuErrorState=RCC_enuINVALID_PLLSRC;
	}
	
	else
	{
		/*PLL Configurations*/
		u32 Loc_u32PLLRCC_PLLCFGR;
		
		/* Store the current PLLCFGR value */
		Loc_u32PLLRCC_PLLCFGR = RCC->PLLCFGR;

		/* Modify the required fields */
		Loc_u32PLLRCC_PLLCFGR &= ~PLL_MASK; // Clear existing bits
		
        /*Set new configuration*/
		Loc_u32PLLRCC_PLLCFGR|=(Loc_u32PLL_P<<PLLP0)|(Loc_u32PLL_Q << PLLQ0)|(Loc_u32PLL_CLKSRC<<PLLSRC)|(Loc_u32PLL_M<<PLLM0)|(Loc_u32PLL_N<<PLLN0); 

		/* Update the PLLCFGR register */
		RCC->PLLCFGR = Loc_u32PLLRCC_PLLCFGR;
	}
	return RCC_enuErrorState;
}

/**
 *@brief : Function to Control(Enable/Disable) peripheral  
 *@param : Copy_u32PeripheralBus: RCC_AHB1/RCC_AHB2/RCC_APB1/RCC_APB2
           Copy_u32Peripheral: Peripheral pin (ex:GPIOBEN)
		   Copy_enuPeriphState: Periph_enuOFF/Periph_enuON
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
                                                                           
 */
RCC_enuErrorStatus_t RCC_enuEnableDisablePeripheral(u32 Copy_u32PeripheralBus, u32 Copy_u32Peripheral, RCC_enuPeriphStatus_t Copy_enuPeriphState )
{
	/*** ERROR handling ***/
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	
	switch(Copy_u32PeripheralBus)
	{
		case RCC_AHB1:
		switch(Copy_enuPeriphState)
		{
			case Periph_enuOFF:
			RCC->AHB1ENR&=~Copy_u32Peripheral;
			break;
			case Periph_enuON:
			RCC->AHB1ENR|=Copy_u32Peripheral;
			break;
		}
		break;
		
		case RCC_AHB2:
		switch(Copy_enuPeriphState)
		{
			case Periph_enuOFF:
			RCC->AHB2ENR&=~Copy_u32Peripheral;
			break;
			case Periph_enuON:
			RCC->AHB2ENR|=Copy_u32Peripheral;
			break;
		}
		break;
		
		case RCC_APB1:
		switch(Copy_enuPeriphState)
		{
			case Periph_enuOFF:
			RCC->APB1ENR&=~Copy_u32Peripheral;
			break;
			case Periph_enuON:
			RCC->APB1ENR|=Copy_u32Peripheral;
			break;
		}
		break;
		
		case RCC_APB2:
		switch(Copy_enuPeriphState)
		{
			case Periph_enuOFF:
			RCC->APB2ENR&=~Copy_u32Peripheral;
			break;
			case Periph_enuON:
			RCC->APB2ENR|=Copy_u32Peripheral;
			break;
		}
		break;	
	}
	
	return RCC_enuErrorState;
}

/**
 *@brief : Function to configure the prescalars of AHB 
 *@param : Copy_u32APB2Prescaler:choose one of the following options :
		  -AHB_NO_PRES
		  -AHB_PRES_2
		  -AHB_PRES_4   
		  -AHB_PRES_8   
		  -AHB_PRES_16  
		  -AHB_PRES_64  
		  -AHB_PRES_128 
		  -AHB_PRES_256 
		  -AHB_PRES_512 
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
          -Invalid AHB Prescalar.                                                               
 */
RCC_enuErrorStatus_t RCC_enuConfigPrescalar_AHB(u32 Copy_u32AHBPrescaler)
{
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	u32 Loc_u32RCC_CFGR;
	/*ERROR HANDLING*/
	if(Copy_u32AHBPrescaler<(AHB_NO_PRES<<4) || Copy_u32AHBPrescaler>(AHB_PRES_512<<4))
	{
		RCC_enuErrorState=RCC_enuINVALID_AHBPRES;
	}
	else
	{
		Loc_u32RCC_CFGR=RCC->CFGR;
		Loc_u32RCC_CFGR&=~(AHBPRE_MASK);
		Loc_u32RCC_CFGR|=Copy_u32AHBPrescaler;
		RCC->CFGR=Loc_u32RCC_CFGR;
	}
	return RCC_enuErrorState;
}
/**
 *@brief : Function to configure the prescalars of APB1 
 *@param : Copy_u32APB2Prescaler:choose one of the following options :
		  -APB1_NO_PRES 
		  -APB1_PRES_2  
		  -APB1_PRES_4 
		  -APB1_PRES_8 
		  -APB1_PRES_16
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
          -Invalid APB1 Prescalar.                                                               
 */
RCC_enuErrorStatus_t RCC_enuConfigPrescalar_APB1(u32 Copy_u32APB1Prescaler)
{
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	u32 Loc_u32RCC_CFGR;
	/*ERROR HANDLING*/
	if(!(Copy_u32APB1Prescaler==APB1_NO_PRES||Copy_u32APB1Prescaler==APB1_PRES_2||Copy_u32APB1Prescaler==APB1_PRES_4||Copy_u32APB1Prescaler==APB1_PRES_8||Copy_u32APB1Prescaler==APB1_PRES_16))
	{
		RCC_enuErrorState=RCC_enuINVALID_APB1PRES;
	}
	else
	{
		Loc_u32RCC_CFGR=RCC->CFGR;
		Loc_u32RCC_CFGR&=~(APB1PRE_MASK);
		Loc_u32RCC_CFGR|=Copy_u32APB1Prescaler;
		RCC->CFGR=Loc_u32RCC_CFGR;
	}
	return RCC_enuErrorState;
}

/**
 *@brief : Function to configure the prescalars of APB2  
 *@param : Copy_u32APB2Prescaler:choose one of the following options :
		  -APB2_NO_PRES 
		  -APB2_PRES_2  
		  -APB2_PRES_4 
		  -APB2_PRES_8 
		  -APB2_PRES_16
 *@return: RCC_enuErrorStatus_t: report error at the following cases:
          -Invalid APB2 Prescalar.                                                               
 */
RCC_enuErrorStatus_t RCC_enuConfigPrescalar_APB2(u32 Copy_u32APB2Prescaler)
{
	RCC_enuErrorStatus_t RCC_enuErrorState= RCC_enuOK;
	u32 Loc_u32RCC_CFGR;
	/*ERROR HANDLING*/
	if(!(Copy_u32APB2Prescaler==APB2_NO_PRES||Copy_u32APB2Prescaler==APB2_PRES_2||Copy_u32APB2Prescaler==APB2_PRES_4||Copy_u32APB2Prescaler==APB2_PRES_8||Copy_u32APB2Prescaler==APB2_PRES_16))
	{
		RCC_enuErrorState=RCC_enuINVALID_APB2PRES;
	}
	else
	{
		Loc_u32RCC_CFGR=RCC->CFGR;
		Loc_u32RCC_CFGR&=~(APB2PRE_MASK);
		Loc_u32RCC_CFGR|=Copy_u32APB2Prescaler;
		RCC->CFGR=Loc_u32RCC_CFGR;	
	}
	return RCC_enuErrorState;
}


