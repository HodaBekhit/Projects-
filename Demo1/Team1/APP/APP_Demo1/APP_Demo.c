/*
 * APP_Demo.c
 * @brief This file contains the implementation of the main application.
 *  Created on: Apr 10, 2024
 *  Author: Eman and Hoda
 */

/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "APP_Demo.h"

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/
/**
 * @typedef uses as edit state is on or off
*/
typedef enum
{
	/* if edit mode is on */
	ON,
	/* if edit mode is off */
	OFF
} EditState_t;
/**
 * @typedef uses as LCD modes it has 3 modes main menu and DateTime and StopWatch
*/
typedef enum
{
	/* if LCD mode is mainMenu */
	MainMenu,
	/* if LCD mode is DateAndTime */
	DateAndTime,
	/* if LCD mode is StopWatch */
	StopWatch
} LCD_Mode_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/
/**
 * @var uses to change it to know what is the state of edit, Is it on or off
*/
EditState_t EditState = OFF;
/**
 * @var to store the value of uart receive
*/
u8 IPC_buffer;
/**
 * @var to know how many i entered the edit mode
*/
u8 EditCount = 0;
/**
 * @var to know if time and date shoulb be displayed or not 0 for no 1 for yes
*/
u8 TimeDateDisp_flag = 0;
/**
 * @var to know if cursor displayed or not 0 for no 1 for yes
*/
u8 CursorOn_flag = 0;
/**************** extern variables ******************/
/**
 * @var to get x-pos from TimeDate files which is cursor rows number
*/
extern u8 X_Pos;
/**
 * @var to get y-pos from TimeDate files which is cursor colomns number
*/
extern u8 Y_Pos;
/**
 * @var to get y-pos from StopWatch files which is values of each digit in stop watch
*/
extern StopWatch_value_t StopWatch_value;
/**************** static variables *****************/
/**
 * @var LCD_Mode
 * @brief Static variable to determine the current LCD mode (MainMenu, DateAndTime, StopWatch).
 */
static LCD_Mode_t LCD_Mode = MainMenu;
/**
 * @var LCD_modechoose
 * @brief Static variable to choose the LCD mode (MainMenu, DateAndTime, StopWatch).
 */
static LCD_Mode_t LCD_modechoose=StopWatch;;
/**
 * @var StartStopCount
 * @brief Static variable to count the number of times start/stop button is pressed.
 */
static u8 StartStopCount = 0;
/**
 * @var PauseContinueCount
 * @brief Static variable to count the number of times pause/continue button is pressed.
 */
static u8 PauseContinueCount = 0;
/**
 * @var StopWatchDisp_flag
 * @brief Static flag to determine if stopwatch should be displayed (0 for no, 1 for yes).
 */
static u8 StopWatchDisp_flag = 0;
/**
 * @var StartStop_flag
 * @brief Static flag to determine if start/stop is activated (0 for stop, 1 for start).
 */
static u8 StartStop_flag = 0;
/**
 * @var PauseContinue_flag
 * @brief Static flag to determine if pause/continue is activated (0 for continue, 1 for pause).
 */
static u8 PauseContinue_flag = 0;
/**
 * @var StartFirst_flag
 * @brief Static flag to indicate if it's the first start of the stopwatch (1 for first start, 0 for not).
 */
static u8 StartFirst_flag = 1;

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

/************************************************** Main Menu *******************************************/
/**
 * @fn APP_LCDMainMenu
 * @brief Function to display the main menu on the LCD.
 * 
 * This function sets the cursor position on the LCD and writes menu options
 * for "Stopwatch" and "Date & Time" using asynchronous LCD functions.
 */
void APP_LCDMainMenu(void)
{
	// Set cursor position to the first row and first column
	LCD_GotoPos_XY_async(0, 0, NULLPTR);
	// Write ">>Stopwatch" on the LCD's first row
	LCD_enuWriteString_asynch(">>Stopwatch", NULLPTR);
	// Set cursor position to the second row and first column
	LCD_GotoPos_XY_async(1, 0, NULLPTR);
	// Write ">>Date & Time" on the LCD's second row
	LCD_enuWriteString_asynch(">>Date & Time", NULLPTR);
	// Reset cursor position to the first row and first column
	LCD_GotoPos_XY_async(0, 0, NULLPTR);
}

/************************************************* cbf ***************************************************/
/**
 * @fn CBFun
 * @brief Callback function to handle LCD mode switching and button presses.
 * 
 * This function is called whenever an interrupt occurs or data is received via IPC.
 * It checks the current LCD mode and IPC buffer to decide the action to be taken.
 */
void CBFun(void)
{
	/* select the chosen mode */
	switch (LCD_Mode)
	{
	case MainMenu:// Handle Main Menu LCD mode
	{
		switch (IPC_buffer) // Check the IPC buffer for commands
		{
		case UP:
			// Navigate to Stopwatch option
			LCD_GotoPos_XY_async(0, 0, NULLPTR);
			/* make the chosen mode is StopWatch*/
			LCD_modechoose = StopWatch;
			break;
		case DOWN_PAUSE_CONTINUE_STOPWATCH:
			// Navigate to Date & Time option
			LCD_GotoPos_XY_async(1, 0, NULLPTR);
			/* make the chosen mode is DateTime*/
			LCD_modechoose = DateAndTime;
			break;
		case OK_MODE:
			// Change LCD mode based on the chosen option
			LCD_Mode = LCD_modechoose;
			//clear LCD screen
			LCD_ClearScreen_async();
			// Update display flags based on selected mode
			if (LCD_modechoose == DateAndTime)
			{
				// Date&Time Display
				TimeDateDisp_flag = 1;
				StopWatchDisp_flag = 0;
			}
			else if (LCD_modechoose == StopWatch)
			{
				LCD_GotoPos_XY_async(0,3,NULLPTR);
				//write StopWatch on LCD screen
	            LCD_enuWriteString_asynch("Stopwatch", NULLPTR);
				// stopwatch display
				StopWatchDisp_flag = 1;
				TimeDateDisp_flag = 0;
			}
			break;
		default:
		/* do nothing */
			break;
		}
	}
	break;
	case DateAndTime: // Handling Date & Time LCD mode
	{
		switch (IPC_buffer)
		{
		case OK_MODE:
		    // Switch to Stopwatch mode
			LCD_Mode = StopWatch;
			//clear LCD screen
			LCD_ClearScreen_async();
			TimeDateDisp_flag = 0;
			CursorOn_flag =0;
			// display StopWatch
			LCD_GotoPos_XY_async(0,3,NULLPTR);
			//write StopWatch on LCD screen
	        LCD_enuWriteString_asynch("Stopwatch", NULLPTR);
			StopWatchDisp_flag = 1;
			break;
		case EDIT:
			// Toggle edit mode and cursor visibility
			if (EditCount % 2 == 0)
			{
				// Enable Cursor
				CursorOn_flag = 1;
				EditState = ON;
				EditCount++;
			}
			else
			{
				// Disable Cursor
				CursorOn_flag = 0;
				EditState = OFF;
				EditCount++;
			}
			break;
		case LEFT_RESET_STOPWATCH:
			// Move cursor left if in edit mode
			if (EditState == ON)
			{
				LCD_MOVE_DIRECTION(Cursor_Left_Shift);
			}
			else
			{
				/* do nothing */
			}
			break;
		case RIGHT_START_STOP_STOPWATCH:
			// Move cursor right if in edit mode
			if (EditState == ON)
			{
			  LCD_MOVE_DIRECTION(Cursor_Right_Shift);
			}
			else
			{
				/* do nothing */
			}
			break;
		case UP:
			// Increment time or date value if in edit mode
			if (EditState == ON)
			{
				LCD_INCDEC_TimeDate(Increment);
			}
			else
			{
				/* do nothing */
			}
			break;
		case DOWN_PAUSE_CONTINUE_STOPWATCH:
			// Decrement time or date value if in edit mode
			if (EditState == ON)
			{
				LCD_INCDEC_TimeDate(Decrement);
			}
			else
			{
				/* do nothing */
			}
			break;
		default:
			/* do nothing */
			break;
		}
	}
	break;

	case StopWatch:// Handling Stopwatch LCD mode
	{
		switch (IPC_buffer)
		{

		case OK_MODE:
			// Switch to Date & Time mode
			LCD_Mode = DateAndTime;
			LCD_ClearScreen_async();
			// display time and date
			TimeDateDisp_flag = 1;
			StopWatchDisp_flag = 0;
			break;
		case RIGHT_START_STOP_STOPWATCH:
			// Toggle Start/Stop flag
			PauseContinue_flag = 0;
			if (StartStopCount % 2 == 0)
			{
				StartStop_flag = 1;
				StartStopCount++;
			}
			else
			{
				StartStop_flag = 0;
				StartStopCount++;
			}
			break;
		case LEFT_RESET_STOPWATCH:
			// Reset Stopwatch and related flags
			StopWatch_Reset();
			StartStop_flag = 0;
			PauseContinue_flag = 0;
			break;
		case DOWN_PAUSE_CONTINUE_STOPWATCH:
			// Toggle Pause/Continue flag
			StartStop_flag = 0;
			if (PauseContinueCount % 2 == 0)
			{
				PauseContinue_flag = 0;
				PauseContinueCount++;
			}
			else
			{
				PauseContinue_flag = 1;
				PauseContinueCount++;
			}
			break;
		default:
		StopWatchDisp_flag = 1;
			break;
		}
	}
	break;
	default:
	/* do nothing */
		break;
	}
}
/************************************************** Stop Watch *******************************************/
/* runs every 100 ms */
/**
 * @fn StopWatch_TestAPP
 * @brief Function to manage the stopwatch logic.
 */
void StopWatch_TestAPP(void)
{
	if ((StartStop_flag == 1))
	{
		StopWatch_Count(&StopWatch_value);

		if (StartFirst_flag == 0)
		{
			StopWatch_Reset();
			StopWatch_Count(&StopWatch_value);
			StartFirst_flag = 1;
		}
	}

	else if (StartStop_flag == 0)
	{
		StartFirst_flag = 0;
	}

	if (PauseContinue_flag == 0)
	{
	}
	else if (PauseContinue_flag == 1)
	{
		StopWatch_Count(&StopWatch_value);
	}

	if (StopWatchDisp_flag == 1)
	{
		LCD_DisableCursor_asynch();
		StopWatch_Display();
	}
}

/*********************************************** app receive from switches ********************************/
/**
 * @fn APP_Control
 * @brief Function to receive inputs from switches and handle accordingly.
 *
 * * This function uses the IPC_ReceiveUSART function to receive switch status
 * updates from USART channel 1. Upon receiving the status, it calls the 
 * CBFun callback function to handle the received data.
 */
void APP_Control(void)
{
	// Receive data from USART channel 1 and pass it to CBFun callback function
	IPC_ReceiveUSART(USART_CH1, &IPC_buffer, 1, CBFun);
}

/*********************************************** app send pressed switches ********************************/
/**
 * @fn APP_Send
 * @brief Function to send pressed switch values via UART.
 * 
 * This function checks the status of different switches and sends their respective
 * values via UART if they are pressed.
 */
void APP_Send(void)
{
	// Initialize switch state to not pressed
	u8 switchState = SWITCH_NOT_PRESSED;
	// Initialize buffer to store switch value
	u8 buffer=0;
	/* Check and send status for OK_MODE switch */
	SWITCH_GetStatus(SWITCH_OK_MODE, &switchState); // Get status of OK_MODE switch
	switch (switchState)
	{
	case SWITCH_PRESSED:
	    buffer = OK_MODE;// Set buffer to OK_MODE value
		IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR);// Send buffer via UART
		switchState = SWITCH_NOT_PRESSED;// Reset switch state
		break;
	case SWITCH_NOT_PRESSED:
	/* do nothing*/
		break;
	default:
	/* do nothing*/
		break;
	}
	/* Check and send status for UP switch */
	SWITCH_GetStatus(SWITCH_UP, &switchState);// Get status of UP switch
	switch (switchState)
	{
	case SWITCH_PRESSED:
        buffer = UP;// Set buffer to UP value
		IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR);// Send buffer via UART
		switchState = SWITCH_NOT_PRESSED;// Reset switch state
		break;
	case SWITCH_NOT_PRESSED:
	/* do nothing*/
		break;
	default:
	/* do nothing*/
		break;
	}
	/* Check and send status for EDIT button */
	SWITCH_GetStatus(SWITCH_EDIT, &switchState);// Get status of EDIT button
	switch (switchState)
	{
	case SWITCH_PRESSED:
	    buffer=EDIT;// Set buffer to EDIT value
		IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR);// Send buffer via UART
		switchState = SWITCH_NOT_PRESSED;// Reset switch state
		break;
	case SWITCH_NOT_PRESSED:
	/* do nothing*/
		break;
	default:
	/* do nothing*/
		break;
	}
	/* Check and send status for RIGHT_START_STOP_STOPWATCH switch */
	SWITCH_GetStatus(SWITCH_RIGHT_START_STOP_STOPWATCH, &switchState);// Get status of RIGHT_START_STOP_STOPWATCH switch
	switch (switchState)
	{
	case SWITCH_PRESSED:
	    buffer=RIGHT_START_STOP_STOPWATCH; // Set buffer to RIGHT_START_STOP_STOPWATCH value
		IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR); // Send buffer via UART
		switchState = SWITCH_NOT_PRESSED; // Reset switch state
		break;
	case SWITCH_NOT_PRESSED:
	/* do nothing*/
		break;
	default:
	/* do nothing*/
		break;
	}
	/* Check and send status for LEFT_RESET_STOPWATCH switch */
	SWITCH_GetStatus(SWITCH_LEFT_RESET_STOPWATCH, &switchState); // Get status of LEFT_RESET_STOPWATCH switch
	switch (switchState)
	{
	case SWITCH_PRESSED:
	    buffer=LEFT_RESET_STOPWATCH;// Set buffer to LEFT_RESET_STOPWATCH value
		IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR); // Send buffer via UART
		switchState = SWITCH_NOT_PRESSED; // Reset switch state
		break;
	case SWITCH_NOT_PRESSED:
	/* do nothing*/
		break;
	default:
	/* do nothing*/
		break;
	}
	/* Check and send status for DOWN_PAUSE_CONTINUE_STOPWATCH switch */
	SWITCH_GetStatus(SWITCH_DOWN_PAUSE_CONTINUE_STOPWATCH, &switchState);// Get status of DOWN_PAUSE_CONTINUE_STOPWATCH switch
	switch (switchState)
	{
	case SWITCH_PRESSED:
	    buffer=DOWN_PAUSE_CONTINUE_STOPWATCH;// Set buffer to DOWN_PAUSE_CONTINUE_STOPWATCH value
		IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR);// Send buffer via UART
		switchState = SWITCH_NOT_PRESSED;// Reset switch state
		break;
	case SWITCH_NOT_PRESSED:
	/* do nothing*/
		break;
	default:
	/* do nothing*/
		break;
	}
}
/*********************************************** to update time values ********************************/
/* runs every 1000 ms */
/**
 * @fn TimeDate_Update
 * @brief Function to update time and date values and display them.
 * This function checks if the time and date should be displayed or if the cursor 
 * should be enabled, and then updates and displays accordingly.
 */
void TimeDate_Update(void)
{
	// Update the time and date values
	TimeDate_Running();
	// Check if time and date should be displayed
	if (TimeDateDisp_flag == 1)
	{
		// Display time and date
		TimeDateDisplay();
	}
	// Check if cursor should be enabled
	if (CursorOn_flag == 1)
	{
		// Enable the cursor asynchronously
		LCD_EnableCursor_asynch();
		// Set the cursor position asynchronously
		LCD_GotoPos_XY_async(X_Pos, Y_Pos, NULLPTR);
	}
}
