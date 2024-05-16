/*
 * TrafficLight.c
 *
 *  Created on: Mar 14, 2024
 *      Author: Dell
 */
#include "STD_LIB/std_types.h"
#include "MCAL/RCC/RCC.h"
#include "MCAL/GPIO/GPIO.h"
#include "HAL/LED/LED.h"



#define TrafficLight_PeriodicityMS 2000
#define GREEN_STATE_TIME_ms 4000
#define YELLOW_STATE_TIME_ms 2000
#define RED_STATE_TIME_ms  6000


typedef enum
{
	GREEN_STATE,
	YELLOW_STATE,
	RED_STATE,
	NO_STATE
}TrafficLights_t;


TrafficLights_t TrafficLights_state=GREEN_STATE;
void TrafficLights_Runnable(void)
{
	static u16 LocCounter_ms=0;
	static TrafficLights_t Prev_state=NO_STATE;
	LocCounter_ms+=TrafficLight_PeriodicityMS;
	switch(TrafficLights_state)
	{
	    case GREEN_STATE:
	    LED_SetStatus(LED_YELLOW, LED_STATE_OFF);
	    LED_SetStatus(LED_GREEN, LED_STATE_ON);
	    LED_SetStatus(LED_RED, LED_STATE_OFF);
	    if(LocCounter_ms==GREEN_STATE_TIME_ms)
	    {
	    	TrafficLights_state=YELLOW_STATE;
	    	Prev_state=GREEN_STATE;
	    	LocCounter_ms=0;
	    }
		break;

	    case YELLOW_STATE:
	    LED_SetStatus(LED_YELLOW, LED_STATE_ON);
	    LED_SetStatus(LED_GREEN, LED_STATE_OFF);
	    LED_SetStatus(LED_RED, LED_STATE_OFF);
	    if(LocCounter_ms==YELLOW_STATE_TIME_ms)
	    {
	    	if(Prev_state==GREEN_STATE)
	    	{
	    		TrafficLights_state=RED_STATE;
	    	}
	    	else if(Prev_state==RED_STATE)
	    	{
	    		TrafficLights_state=GREEN_STATE;
	    	}
	    	Prev_state=YELLOW_STATE;
	    	LocCounter_ms=0;
	    }
		break;

	    case RED_STATE:
	    LED_SetStatus(LED_YELLOW, LED_STATE_OFF);
	    LED_SetStatus(LED_GREEN, LED_STATE_OFF);
	    LED_SetStatus(LED_RED, LED_STATE_ON);
	    if(LocCounter_ms==RED_STATE_TIME_ms)
	    {
	    	TrafficLights_state=YELLOW_STATE;
	    	Prev_state=RED_STATE;
	    	LocCounter_ms=0;
	    }
		break;
	    default:
	    break;

	}

}
