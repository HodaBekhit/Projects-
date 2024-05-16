/*
 * scheduler.h
 *
 *  Created on: Mar 12, 2024
 *      Author: Dell
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_
#include "STD_LIB/std_types.h"

typedef void (*runnable_cb_t) (void);

typedef enum
{
	Sched_OK,
	Sched_NOK
}Sched_ErrorStatus_t;

// for any configurations configurable by the user
typedef struct
{
	char *name;
	u32 FirstDelay_ms;
	u32 periodicity_ms;
	runnable_cb_t cb;
} runnable_t;

//Sched_ErrorStatus_t scheduler_registerrunnable(runnable_t *runnable);

/*Function to initialize the systick timer */
void scheduler_init(void);

/*Function to start systick timer*/
void scheduler_start(void);

#endif /* SCHEDULER_H_ */
