#include "HAL/LCD/LCD.h"
#include "Stopwatch.h"

StopWatch_value_t StopWatch_value;
/************************** setters ****************************/
/* reset function when reset button pressed */
StopWatch_ErrorState_t StopWatch_Reset(void){
    StopWatch_ErrorState_t Loc_errorState = StopWatch_OK;
    StopWatch_value.millisec = 0;
    StopWatch_value.sec = 0;
    StopWatch_value.min = 0;
    StopWatch_value.hour = 0;
    return Loc_errorState;
}

StopWatch_ErrorState_t StopWatch_Count(StopWatch_value_t* StopWatch_values){
    StopWatch_ErrorState_t Loc_errorState = StopWatch_OK;
    if(StopWatch_value.millisec < 9)
    {
        StopWatch_value.millisec++;
    }
    else
    {
        StopWatch_value.sec++;
        StopWatch_value.millisec = 0;
        if(StopWatch_value.sec >59)
        {
            StopWatch_value.min++;
            StopWatch_value.sec = 0;
            if(StopWatch_value.min > 59)
            {
                StopWatch_value.hour++;
                StopWatch_value.min = 0;
                if(StopWatch_value.hour > 23)
                {
                    StopWatch_value.hour = 0;
                    Loc_errorState = StopWatch_TimeExceeded;
                }
            }
        }
    }
    StopWatch_values->millisec = StopWatch_value.millisec;
    StopWatch_values->sec = StopWatch_value.sec;
    StopWatch_values->min = StopWatch_value.min;
    StopWatch_values->hour = StopWatch_value.hour;
    return Loc_errorState;
}

void StopWatch_Pause(void){
    /* DO NOTHING */
} 

void StopWatch_Display(void)
{
	/*LCD_GotoPos_XY_async(0,3,NULLPTR);
	LCD_enuWriteString_asynch("Stopwatch", NULLPTR);*/
	LCD_GotoPos_XY_async(1,3,NULLPTR);
    if(StopWatch_value.hour == 0){
		    
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		else if(StopWatch_value.hour < 10){
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteNumber_asynch(StopWatch_value.hour, NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		else{
			
			LCD_enuWriteNumber_asynch(StopWatch_value.hour, NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		if(StopWatch_value.min == 0){
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		else if(StopWatch_value.min < 10){
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteNumber_asynch(StopWatch_value.min, NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		else{
			
			LCD_enuWriteNumber_asynch(StopWatch_value.min, NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		if(StopWatch_value.sec == 0){
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		else if(StopWatch_value.sec < 10){
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteNumber_asynch(StopWatch_value.sec, NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		else{
			
			LCD_enuWriteNumber_asynch(StopWatch_value.sec, NULLPTR);
			LCD_enuWriteString_asynch(":", NULLPTR);
		}
		if(StopWatch_value.millisec == 0){
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteString_asynch("0", NULLPTR);
			
		}
		else if(StopWatch_value.millisec<0)
		{
			
			LCD_enuWriteString_asynch("0", NULLPTR);
			LCD_enuWriteNumber_asynch(StopWatch_value.millisec, NULLPTR);
			
		}
		else{
			
			LCD_enuWriteNumber_asynch(StopWatch_value.millisec, NULLPTR);
			
		}
		
}
/************************** getters *****************************/


