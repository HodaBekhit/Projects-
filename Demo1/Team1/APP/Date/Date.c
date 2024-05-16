#include "Date.h"

Date_value_t Date_value;
/************************** setters ****************************/
/* reset function when reset button pressed */
void Date_Reset(void){
    Date_value.Day_index1 = 1;
    Date_value.Day_index2 = 0;
    Date_value.Month_index1 = 1;
    Date_value.Month_index2 = 0;
    Date_value.Year_index1 = 0;
    Date_value.Year_index2 = 0;
    Date_value.Year_index3 = 0;
    Date_value.Year_index4 = 2;
}

/* set time function when edit button pressed and others */
void Date_Set(Date_index_t index, Date_action_t incdec){
    /*Day_index1*/
    if(Day_index1==index && incdec==increment){
        if(Date_value.Day_index1 <9)
        {
        Date_value.Day_index1++;
        }
        else
        {
        }
    }
    else if(Day_index1==index && incdec==decrement){
        if(Date_value.Day_index1>0)
        {
            Date_value.Day_index1--;
        }
        else
        {
        }
    }
/* digit2 */ 
    if(Day_index2==index && incdec==increment){
        if(Date_value.Day_index2 <3)
        {
        Date_value.Day_index2++;
        }
        else
        {
        }
    }
    else if(Day_index2==index && incdec==decrement){
        if(Date_value.Day_index2>0)
        {
            Date_value.Day_index2--;
        }
        else
        {
        }
    }
 /* digit1 month 01:09&10:12*/ 
    if(Month_index1==index && incdec==increment){
        if(Date_value.Month_index1 <9)
        {
        Date_value.Month_index1++;
        }
        else
        {
        }
    }
    else if(Month_index1==index && incdec==decrement){
        if(Date_value.Month_index1 >0)
        {
        Date_value.Month_index1--;
        }
        else
        {
        }
    }      
    /* digit2 month    1:9 if digit2=0 0:2     if digit2=1 */ 
     if(Month_index2==index && incdec==increment){
        if(Date_value.Month_index2 <1)
        {
        Date_value.Month_index2++;
        }
        else
        {
        }
    }
    else if(Month_index2==index && incdec==decrement){
        if(Date_value.Month_index2 >0)
        {
        Date_value.Month_index2--;
        }
        else
        {
        }
    }   
     /* digit1 year */ 
    if(Year_index1==index && incdec==increment){
        if(Date_value.Year_index1<9)
        {
        Date_value.Year_index1++;
        }
        else
        {
        }
    }
    else if(Year_index1==index && incdec==decrement){
        if(Date_value.Year_index1>0)
        {
            Date_value.Year_index1--;
        }
        else
        {
        }
    }    
     /* digit2 year */ 
    if(Year_index2==index && incdec==increment){
        if(Date_value.Year_index2<9)
        {
        Date_value.Year_index2++;
        }
        else
        {
        }
    }
    else if(Year_index2==index && incdec==decrement){
        if(Date_value.Year_index2>0)
        {
            Date_value.Year_index2--;
        }
        else
        {
        }
    }  
     /* digit3 year */ 
    if(Year_index3==index && incdec==increment){
        if(Date_value.Year_index3<9)
        {
        Date_value.Year_index3++;
        }
        else
        {
        }
    }
    else if(Year_index3==index && incdec==decrement){
        if(Date_value.Year_index3>0)
        {
            Date_value.Year_index3--;
        }
        else
        {
        }
    }   
     /* digit4 year */ 
    if(Year_index4==index && incdec==increment){
        if(Date_value.Year_index4<9)
        {
        Date_value.Year_index4++;
        }
        else
        {
        }
    }
    else if(Year_index4==index && incdec==decrement){
        if(Date_value.Year_index4>2)
        {
            Date_value.Year_index4--;
        }
        else
        {
        }
    }
    /* February in a leap year (29 days) */
    u8 year = Date_value.Year_index1 + Date_value.Year_index2 * 10 + Date_value.Year_index3 * 100 + Date_value.Year_index4 * 1000;
    if (Date_value.Month_index1 == 2 && Date_value.Month_index2 == 0 && year % 4 == 0) {
        if (Date_value.Day_index1 + Date_value.Day_index2*10 > 29) {
            Date_value.Day_index1 = 9;
            Date_value.Day_index2 = 2;
        }
    }
    /* February in a leap year (28 days) */
    else if (Date_value.Month_index1 == 2 && Date_value.Month_index2 == 0 && year % 4 != 0) {
        if (Date_value.Day_index1  + Date_value.Day_index2* 10 > 28) {
            Date_value.Day_index1 = 8;
            Date_value.Day_index2 = 2;
        }
    }   
    /* Months with 30 days (April, June, September, November) */
    else if ((Date_value.Month_index1 == 4 || Date_value.Month_index1 == 6 || Date_value.Month_index1 == 9 || Date_value.Month_index1 == 11) && Date_value.Month_index2 == 0) {
        if (Date_value.Day_index1  + Date_value.Day_index2* 10 > 30) {
            Date_value.Day_index1 = 0;
            Date_value.Day_index2 = 3;
        }
    }
    /* Months with 31 days (January, March, May, July, August, October, December) */
    else {
        if (Date_value.Day_index1  + Date_value.Day_index2* 10 > 31) {
            Date_value.Day_index1 = 1;
            Date_value.Day_index2 = 3;
        }
    }

    //  /* month 2 year%4==0 29*/
    // u8 year =Date_value.Year_index1+Date_value.Year_index2*10+Date_value.Year_index3*100+Date_value.Year_index4*10000;
    // if(Date_value.Month_index1==2&&Date_value.Month_index2==0 &&year%4==0){
    //     if(Date_value.Day_index1+Date_value.Day_index2*10>29){
    //         Date_value.Day_index1 = 2;
    //         Date_value.Day_index2 = 9;
    //     }
    // }
    // else if(Date_value.Month_index1==2&&Date_value.Month_index2==0 &&year%4!=0){
    // if(Date_value.Day_index1+Date_value.Day_index2*10>28){
    //         Date_value.Day_index1 = 2;
    //         Date_value.Day_index2 = 8;
    //     }
    // }
    // if((Date_value.Month_index2==0&&(Date_value.Month_index1==4||Date_value.Month_index1==6||Date_value.Month_index1==9))
    //  ||(Date_value.Month_index2==1&&Date_value.Month_index1==1)){
    //     if(Date_value.Day_index1+Date_value.Day_index2*10>30){
    //         Date_value.Day_index1 = 0;
    //         Date_value.Day_index2 = 3;
    //     }
    // }
    // if((Date_value.Month_index2==0&&(Date_value.Month_index1==1||Date_value.Month_index1==3||Date_value.Month_index1==5||Date_value.Month_index1==7||Date_value.Month_index1==8))
    //  ||(Date_value.Month_index2)){
    //     if(Date_value.Day_index1+Date_value.Day_index2*10>31){
    //         Date_value.Day_index1 = 1;
    //         Date_value.Day_index2 = 3;
    //     }
    // }
}