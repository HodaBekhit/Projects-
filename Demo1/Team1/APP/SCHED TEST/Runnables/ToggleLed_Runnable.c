/*
 * app1.c
 *
 *  Created on: Mar 20, 2024
 *      Author: Dell
 */
#include "HAL/LED/LED.h"
void ToggleRedRunnable(void)
{
	// Initial state set to LED_STATE_OFF
	static u32 status = LED_STATE_OFF;
	// Toggle LED status between LED_STATE_OFF and LED_STATE_ON
	status ^= (LED_STATE_ON | LED_STATE_OFF);
	LED_SetStatus(LED_RED, status);
}
