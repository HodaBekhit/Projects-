#ifndef DATE_H_
#define DATE_H_

#include "STD_LIB/std_types.h"
/**
 * @brief 
 * 
 */
typedef struct {
    u8 Day_index1;
    u8 Day_index2;
    u8 Month_index1;
    u8 Month_index2;
    u8 Year_index1;
    u8 Year_index2;
    u8 Year_index3;
    u8 Year_index4;
}Date_value_t;
/**
 * @brief error state types
 * 
 */
typedef enum{
    Date_OK,
    Date_TimeExceeded
}Date_ErrorState_t;

typedef enum{
    increment,
    decrement,
    no_action
}Date_action_t;

typedef enum{
    Day_index1,
    Day_index2,
    Month_index1,
    Month_index2,
    Year_index1,
    Year_index2,
    Year_index3,
    Year_index4
}Date_index_t;
/************************** setters ****************************/
/* reset function when reset button pressed */
void Date_Reset(void);

void Date_Set(Date_index_t index, Date_action_t incdec);

#endif