/*
 * NVIC.c
 *
 * Created: 3/5/2024 3:26:10 PM
 *  Author: Eman
 */ 
#include "NVIC.h"
typedef struct
{
    volatile u32 NVIC_ISER[8];
	volatile u32 Reserved[24];
	volatile u32 NVIC_ICER[8];
	volatile u32 Reserved1[24];
	volatile u32 NVIC_ISPR[8];
	volatile u32 Reserved2[24];
	volatile u32 NVIC_ICPR[8];
	volatile u32 Reserved3[24];
	volatile u32 NVIC_IABR[8];
	volatile u32 Reserved4[56];
	volatile u8 NVIC_IPR[240]; /* u8 because it's byte access or may use u32 with casting to u8*/
    volatile u32 Reserved5[580];
	volatile u32 NVIC_STIR;
	
}NVIC_Register_t;

typedef struct
{
    
	/* empty till now */

}SCB_Register_t;

#define NVIC                     ((NVIC_Register_t*)0xE000E100)
#define SCB                      ((SCB_Register_t*)0xE000ED00)
#define SCB_BASE                 (0xE000E008)   // 0X0C
#define AIRCR                    (*(volatile u32*)0xE000ED0C)
#define VECTKEY_MASK             0x05FA0000
#define NVIC_STIR                (*(volatile u32*)0xE000EE00)
#define INT_NUMBER               240
#define REGISTER_SIZE            32
#define INT_PERIORITY_PER_REG    4
#define INT_PERIORITY_BITS       4
#define NULL                     ((void *)0)

#define NVICMASKAIRCR           0x0000F8FF

#define SUBGROUP_BITS            1 // 1 bit
/************************************************************************************
 *                                       functions                                  *
 * **********************************************************************************/

NVIC_ErrorStatus_t NVIC_EnableInterrupt(u8 Copy_InterruptID){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_InterruptID > INT_NUMBER){
        ErrorStatusLocVar = NVIC_WrongINTNumber;
    }
    else{
        NVIC->NVIC_ISER[Copy_InterruptID/REGISTER_SIZE] |= (1<<(Copy_InterruptID%REGISTER_SIZE));
    }
    return ErrorStatusLocVar;
}

NVIC_ErrorStatus_t NVIC_DisableInterrupt(u8 Copy_InterruptID){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_InterruptID > INT_NUMBER){
        ErrorStatusLocVar = NVIC_WrongINTNumber;
    }
    else{
        NVIC->NVIC_ICER[Copy_InterruptID/REGISTER_SIZE] |= (1<<(Copy_InterruptID%REGISTER_SIZE));
    }
    return ErrorStatusLocVar;
}

NVIC_ErrorStatus_t NVIC_SetPendingInterrupt(u8 Copy_InterruptID){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_InterruptID > INT_NUMBER){
        ErrorStatusLocVar = NVIC_WrongINTNumber;
    }
    else{
        NVIC->NVIC_ISPR[Copy_InterruptID/REGISTER_SIZE] |= (1<<(Copy_InterruptID%REGISTER_SIZE));
    }
    return ErrorStatusLocVar;
}

NVIC_ErrorStatus_t NVIC_ClearPendingInterrupt(u8 Copy_InterruptID){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_InterruptID > INT_NUMBER){
        ErrorStatusLocVar = NVIC_WrongINTNumber;
    }
    else{
        NVIC->NVIC_ICPR[Copy_InterruptID/REGISTER_SIZE] |= (1<<(Copy_InterruptID%REGISTER_SIZE));
    }
    return ErrorStatusLocVar;
}

NVIC_ErrorStatus_t NVIC_GetStatusInterrupt(u8 Copy_InterruptID, u8* Ptr_InterruptStatus){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_InterruptID > INT_NUMBER){
        ErrorStatusLocVar = NVIC_WrongINTNumber;
    }
    else if (Ptr_InterruptStatus == NULL){
        ErrorStatusLocVar = NVIC_NULLPOINTER;
    }
    else{
        * Ptr_InterruptStatus = ((NVIC->NVIC_IABR[Copy_InterruptID/REGISTER_SIZE])>>(Copy_InterruptID%REGISTER_SIZE))&1;
    }  
    return ErrorStatusLocVar;
}

NVIC_ErrorStatus_t NVIC_SetPriority(s8 Copy_InterruptID, u8 Copy_Prioty){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_InterruptID < WWDG_IRQn || Copy_InterruptID > INT_NUMBER){
        ErrorStatusLocVar = NVIC_WrongINTNumber;
    }
    else{
        NVIC->NVIC_IPR[Copy_InterruptID/INT_PERIORITY_PER_REG] |= Copy_Prioty << ((Copy_InterruptID%INT_PERIORITY_PER_REG) << (INT_PERIORITY_BITS + SUBGROUP_BITS));
    }  
    return ErrorStatusLocVar;
}

NVIC_ErrorStatus_t NVIC_SetSubGroupbits(u32 Copy_SubGroupbits){
    NVIC_ErrorStatus_t ErrorStatusLocVar = NVIC_Ok;
    if(Copy_SubGroupbits < SCB_PRI_GROUP_0 || Copy_SubGroupbits > SCB_PRI_GROUP_7){
        ErrorStatusLocVar = NVIC_WrongGroupPriority; /* supposed not error but till now it's*/
    }
    else{
        u32 Loc_Temp = AIRCR;
        Loc_Temp &= NVICMASKAIRCR;
        Loc_Temp |= Copy_SubGroupbits;
        AIRCR = Loc_Temp;
    }  
    return ErrorStatusLocVar;
}
