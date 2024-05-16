/*
 * Runnables_List.c
 *
 *  Created on: Mar 14, 2024
 *      Author: Dell
 */
#include "Runnables_List.h"
#include "scheduler.h"
extern void LCD_TASK(void);
extern void APP_Control(void);
extern void SWITCH_Runnable(void);
extern void TimeDate_Update(void);
extern void StopWatch_TestAPP(void);
extern void APP_Control(void);
extern void APP_Send(void);

const runnable_t RunnablesList[_Runnables_NUM] = {
	[LCD_TASK_RUN] = {
		.name = {"LCD Runnable"},
		.FirstDelay_ms = 3,
		.periodicity_ms = 2,
		.cb = LCD_TASK},

	[HSwitchRunnable] = {
		.name = {"Switch Runnable"},
		.FirstDelay_ms = 6,
		.periodicity_ms = 5,
		.cb = SWITCH_Runnable},

	[STOP_WATCH] = {
		.name = {"update stopwatch"},
		.FirstDelay_ms = 100, 
		.periodicity_ms = 100,
		.cb = StopWatch_TestAPP},

	[DISPLAY_APP] = {
		.name = {"display action based on received uart frame"}, 
		.FirstDelay_ms = 131, 
		.periodicity_ms = 125,
		.cb = APP_Control},

	[AppButtonsRun] = {
		.name = {"send uart frames based on pressed switch"},
		.FirstDelay_ms = 150,
		.periodicity_ms = 152, 
		.cb = APP_Send},

	[DATE_TIME] = {
		.name = {"update date and time"},
		.FirstDelay_ms = 1004, 
		.periodicity_ms = 1000,
		.cb = TimeDate_Update}
		};
