/*
 * STK.h
 *
 *  Created on: Mar 9, 2024
 *      Author: Dell
 */

#ifndef STK_H_
#define STK_H_
#include "STD_LIB/std_types.h"

#define STK_OneTime  1
#define STK_Periodic 2

typedef void (*STK_cbf_t) (void);

typedef enum{
	STK_OK,
	STK_NOK
}STK_ErrorStatus_t;


/* functions prototypes */

/*********************************  STK_Init(void); **********************************************************
 * @brief:    Initializes the SysTick timer. (Set clock source & interrupt state) ---------------------------*
 * @return:   Error status (Report Error if user configured invalid clock source.) --------------------------*
 ************************************************************************************************************/
STK_ErrorStatus_t STK_Init(void);

/********************************* STK_Start *****************************************************************
 * @brief:    Enables the SysTick timer. --------------------------------------------------------------------*
 * @param_in: STK_Mode, Can be: -----------------------------------------------------------------------------*
 *            - STK_OneTime ---------------------------------------------------------------------------------*
 *            - STK_Periodic --------------------------------------------------------------------------------*
 ************************************************************************************************************/
void STK_Start(u8 STK_Mode);

/********************************* STK_Stop ******************************************************************
 * @brief:    Disables the SysTick timer. -------------------------------------------------------------------*
 ************************************************************************************************************/
void STK_Stop(void);

/********************************* STK_SetTime_ms ************************************************************
 * @brief:    Sets the time for systick timer in milliseconds. ----------------------------------------------*
 * @param_in:  CopyTime_ms: The time interval in milliseconds. ----------------------------------------------*
 * @return:   Error status (Report Error if the load value exceeds the register limit). ---------------------*
 ************************************************************************************************************/
STK_ErrorStatus_t STK_SetTime_ms(u32 CopyTime_ms);

/********************************* STK_IsExpired *************************************************************
 * @brief:    Checks if the SysTick interrupt has expired. --------------------------------------------------*
 * @param_in:  No parameters. -------------------------------------------------------------------------------*
 * @return:   Error Status. ---------------------------------------------------------------------------------*
 ************************************************************************************************************/
STK_ErrorStatus_t STK_IsExpired(void);

/********************************* STK_RegisterCallback ******************************************************
 * @brief:    Registers a callback function to be called upon SysTick interrupt. ----------------------------*
 * @param_in:  cbf: Callback function pointer. --------------------------------------------------------------*
 * @return:   Error status (Report error if null pointer). --------------------------------------------------*
 ************************************************************************************************************/
STK_ErrorStatus_t STK_RegisterCallback(STK_cbf_t cbf);

#endif /* STK_H_ */
