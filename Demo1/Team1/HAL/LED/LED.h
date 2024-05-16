#ifndef HAL_LED_LED_H_
#define HAL_LED_LED_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_LIB/std_types.h"
#include "LED_cfg.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
#define LED_STATE_ON  0x00000001
#define LED_STATE_OFF 0X00010000
#define LED_CONN_FWD  0x00000000
#define LED_CONN_REV  0x00010001

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/*** Led configurations ***/
typedef struct{
	void* LED_Port;
	u32 LED_Pin;
	u8  LED_Conn;
	u32 LED_Mode;
	u32 LED_Speed;
	}LED_cfg_t;
	
/*** Error Status ***/
typedef enum
{
	LED_OK,
	LED_InvalidLed,
	LED_InvalidStatus
	}LED_ERRORSTATUS_t;

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/

/**
 * @brief: Initializes the LED according to external LED configuration.
 *
 * @return: LED_ERRORSTATUS_t :
 *         - LED_OK
 *         - GPIO_ERRORStatus
 */
LED_ERRORSTATUS_t LED_Init(void);

/**
 * @brief: Sets the status of the specified LED.
 *
 * @param: - Copy_LED: The name of the LED whose status will be set.
 *         - Copy_Status: The status to set for the LED (LED_STATE_ON or LED_STATE_OFF).
 *
 * @return: LED_ERRORSTATUS_t:
 *         - LED_OK
 *         - LED_InvalidLed
 *         - LED_InvalidStatus
 */
LED_ERRORSTATUS_t LED_SetStatus(u32 Copy_LED, u32 Copy_Status);
































#endif // HAL_LED_LED_H_