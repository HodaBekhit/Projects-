/**
 * @file APP_Demo.h
 * @author Eman & Hoda
 * @brief  this is the header file of App_demo which has the application which will use in main directly.
 * @version 0.1
 * @date 2024-04-10
 * 
 * @copyright Copyright (c) 2024
 * 
 */
#ifndef APP_DEMO_H_
#define APP_DEMO_H_

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_LIB/std_types.h"
#include "HAL/LCD/LCD.h"
#include "HAL/LED/LED.h"
#include "MCAL/USART/USART.h"
#include "APP/StopWatch/StopWatch.h"
#include "APP/TimeDate/TimeDate.h"
#include "HAL/IPC/IPC.h"
#include "HAL/SWITCH/SWITCH.h"
#include "SCHED/scheduler.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/
/**
 * @brief for ok or mode switch ok for choose if DateTime or StopWatch if it in mainmenu
 *        Mode to switch between TimeDate and StopWatch
 */
#define OK_MODE                       '1'
/**
 * @brief up to move to the above line
 *        
 */
#define UP                            '2'
/**
 * @brief to switch to edit mode
 *        
 */
#define EDIT                          '3'
/**
 * @brief up to move to the bottom line
 *        and reset in stpwatch
 *        
 */
#define RIGHT_START_STOP_STOPWATCH    '4'
/**
 * @brief up to move to the bottom line
 *        and reset in stpwatch     
 */
#define LEFT_RESET_STOPWATCH          '5'
/**
 * @brief up to move to the bottom line
 *        and pause or continue switch in stopwatch
 *        
 */
#define DOWN_PAUSE_CONTINUE_STOPWATCH '6'

/********************************************************************************************************/
/************************************************APIs****************************************************/
/********************************************************************************************************/
/**
 * @brief :to display main menu of Date and type and stopwatch
 * @param :no need
 * @return:no need
 */
void APP_LCDMainMenu(void);


#endif