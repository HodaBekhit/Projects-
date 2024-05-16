#ifndef STOPWATCH_H_
#define STOPWATCH_H_

#include "STD_LIB/std_types.h"
/**
 * @brief 
 * 
 */
typedef struct {
    u8 millisec;
    u8 sec;
    u8 min;
    u8 hour;
}StopWatch_value_t;
/**
 * @brief error state types
 * 
 */
typedef enum{
    StopWatch_OK,
    StopWatch_TimeExceeded
}StopWatch_ErrorState_t;

/**
 * @brief to reset stop watch and start from 00:00:00
 * @param void
 * @return error state 
 */
StopWatch_ErrorState_t StopWatch_Reset(void);
/**
 * @brief to count every 100msec
 * @param pointer to stopwatch struct that have time values
 * @return error state 
 */
StopWatch_ErrorState_t StopWatch_Count(StopWatch_value_t* StopWatch_values);

void StopWatch_Display(void);
#endif