/********************************************************************************************************/
/************************************************Includes************************************************/
/********************************************************************************************************/
#include "STD_LIB/std_math.h"
#include "LCD.h"

/********************************************************************************************************/
/************************************************Defines*************************************************/
/********************************************************************************************************/

/* Periodicity in milliseconds for LCD_TASK */
#define LCD_Periodicityms       2U

/* Commands Macros */
#define LCD_FUNCSET_8BIT           0x38
#define LCD_DISPON_CURSON_BLOFF    0x0E
#define LCD_DISPON_CURSOFF_BLOFF   0x0C
#define LCD_DISPON_CURSON_BLON     0x0F
#define LCD_CLR                    0x01
#define LCD_ENTRYMODE_SETINC_SHOFF 0x06
#define LCD_CLR                    0x01

/* Max Buffer Requests*/
#define MAXBufferSize 100
/* Line Numbers */
#define FirstLine               0
#define SecondLine              1

/* Pin States */
#define PinHigh                 0x00000001
#define PinLow                  0x00010000

/********************************************************************************************************/
/************************************************Types***************************************************/
/********************************************************************************************************/

/* Enumeration for LCD State */
typedef enum
{
    Idle,
    Busy
} LCDState_t;

/* Enumeration for Enable Pin State */
typedef enum
{
    EN_High,
    EN_Low
} EnableState_t;

/* Enumeration for LCD Mode */
typedef enum
{
    Init,
    Operation,
    Off
} LCDMode_t;

/* Enumeration for LCD Initialization State */
typedef enum
{
    PowerON,
    FunctionSet,
    DisplayControl,
    DisplayClear,
    EntryModeSet,
    End
} LCD_InitState_t;

/* Enumeration for LCD Operation Type */
typedef enum
{
    WriteString,
    WriteNumber,
    Clear,
    SetPos,
	ShiftCursor,
	DisableCursor,
	EnableCursor,
    None
} LCDOperation_t;


/* Structure for LCD Request */
typedef struct
{
    LCDState_t State;   /* Busy, Idle */
    LCDOperation_t Type; /* WriteString/WriteNumber/clear/setpos */
    LCD_CBF cb;
} LCDRequest_t;

/* Structure for Write String Request */
typedef struct
{
    const char* data;
    u16 Length;
    u8 CurrIdx;
} StringRequest_t;

/* Structure for Write Number Request */
typedef struct
{
    u16 Number;
    u16 Length;
    u8 CurrIdx;
} NumberRequest_t;

/* Structure for shift cursor Request */
typedef struct
{
    u8 Direction;
} ShiftCursorRequest_t;

/* Structure for Write Position Request */
typedef struct
{
    u8 X_Pos;
    u8 Y_Pos;
} PositionRequest_t;

/********************************************************************************************************/
/************************************************Variables***********************************************/
/********************************************************************************************************/

/* External Declaration of LCD Pins Configuration */
extern const LCD_PinCgf_t LCD_PINS_CFG[LCD_PINS_NUM];
static LCDRequest_t LCDRequest[MAXBufferSize];
static StringRequest_t WriteStrReq[MAXBufferSize];
static PositionRequest_t SetPosReq[MAXBufferSize];
static NumberRequest_t WriteNumReq[MAXBufferSize];
static ShiftCursorRequest_t ShiftCurReq[MAXBufferSize];
static LCDMode_t LCDMode = Off;
static EnableState_t EnableState = EN_High;
LCD_CBF fun=NULLPTR;
u8 CurrRunnablePtr=0;
u8 RequestPtr=0;

/********************************************************************************************************/
/*****************************************Static Functions Prototype*************************************/
/********************************************************************************************************/
static void LCD_enuWriteCommand(u8 Copy_u8Command);
static void LCD_enuWriteData(u8 Copy_u8Data);
static void LCD_WriteStringProc(void);
static void LCD_WriteNumberProc(void);
static void LCD_ClearProc(void);
static void LCD_SetPosProc(void);
static void LCD_ShiftCursorProc(void);
static void LCD_DisableCursorProc(void);
static void LCD_EnableCursorProc(void);
static void InitSM(void);

/********************************************************************************************************/
/*********************************************APIs Implementation****************************************/
/********************************************************************************************************/

void LCD_InitPins(void)
{
	GPIO_PinCfg_t Loc_LCD;
	u8 Idx=0;
	for(Idx=0;Idx<LCD_PINS_NUM;Idx++)
	{
		Loc_LCD.GPIO_MODE=output_PP;
		Loc_LCD.GPIO_SPEED=High_speed;
		Loc_LCD.GPIO_PORT=LCD_PINS_CFG[Idx].PORT;
		Loc_LCD.GPIO_PIN=LCD_PINS_CFG[Idx].PIN;
		GPIO_InitPin(&Loc_LCD);
	}
}

void LCD_init_asynch(LCD_CBF cbf )
{
	u16 idx=0;
    /*Initialize State to Be Idle*/
    for( idx=0;idx<MAXBufferSize;idx++)
    {
    	LCDRequest[idx].State=Idle;
    }

	LCDMode=Init;
	fun=cbf;
}

LCD_enumError_t LCD_enuWriteString_asynch(const char * string, LCD_CBF cbf)
{

	LCD_enumError_t Ret_LCDError=LCD_enuOk;
	if(string==NULLPTR)
	{
		Ret_LCDError=LCD_enuNullPtr;
	}
	else
	{
		u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
    if (LCDRequest[index].State == Idle)
    {
        // Save user data
        WriteStrReq[index].data = string;
        WriteStrReq[index].CurrIdx = 0;
        // Edit LCD Request Type and State
        LCDRequest[index].Type = WriteString;
        LCDRequest[index].State = Busy;
        LCDRequest[index].cb = cbf;
    }
    // Increment index and reset to 0 if it exceeds MAXBufferSize
    RequestPtr = (RequestPtr + 1) % MAXBufferSize;
	}
	return Ret_LCDError;
}

LCD_enumError_t LCD_GotoPos_XY_async(u8 Copy_u8X, u8 Copy_u8Y,LCD_CBF cbf)
{

	LCD_enumError_t Ret_LCDError=LCD_enuOk;
	if (Copy_u8X>1 || Copy_u8Y>15)
	{
		Ret_LCDError=LCD_enuWrongPosition;
	}
	else
	{
		u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
		if (LCDRequest[index].State == Idle)
		{
			// Save user data
			SetPosReq[index].X_Pos=Copy_u8X;
			SetPosReq[index].Y_Pos=Copy_u8Y;
			//Edit LCD Request Type and State
			LCDRequest[index].Type=SetPos;
			LCDRequest[index].State=Busy;
			LCDRequest[index].cb=cbf;
		}
	    // Increment index and reset to 0 if it exceeds MAXBufferSize
		RequestPtr = (RequestPtr + 1) % MAXBufferSize;
	}
	return Ret_LCDError;
}

void LCD_ClearScreen_async(void)
{
	u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
	if(LCDRequest[index].State==Idle)
	{
		//Edit LCD Request Type and State
	    LCDRequest[index].Type=Clear;
	    LCDRequest[index].State=Busy;
	}
    // Increment index and reset to 0 if it exceeds MAXBufferSize
	RequestPtr = (RequestPtr + 1) % MAXBufferSize;
}
void LCD_enuWriteNumber_asynch(u16 Copy_u8Number,LCD_CBF cbf)
{
	u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
	if(LCDRequest[index].State==Idle)
	{
		//save user data
		WriteNumReq[index].Number=Copy_u8Number;
		//Edit LCD Request Type and State
		LCDRequest[index].Type=WriteNumber;
		LCDRequest[index].State=Busy;
		LCDRequest[index].cb = cbf;
	}
	// Increment index and reset to 0 if it exceeds MAXBufferSize
	RequestPtr = (RequestPtr + 1) % MAXBufferSize;
}

void LCD_ShiftCursor_asynch(u8 direction)
{
	u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
	if(LCDRequest[index].State==Idle)
	{
		//save user data
		ShiftCurReq[index].Direction=direction;
		//Edit LCD Request Type and State
		LCDRequest[index].Type=ShiftCursor;
		LCDRequest[index].State=Busy;
	}
	// Increment index and reset to 0 if it exceeds MAXBufferSize
	RequestPtr = (RequestPtr + 1) % MAXBufferSize;
}

void LCD_DisableCursor_asynch(void)
{
	u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
	if(LCDRequest[index].State==Idle)
	{
		//Edit LCD Request Type and State
	    LCDRequest[index].Type=DisableCursor;
	    LCDRequest[index].State=Busy;
	}
    // Increment index and reset to 0 if it exceeds MAXBufferSize
	RequestPtr = (RequestPtr + 1) % MAXBufferSize;
}

void LCD_EnableCursor_asynch(void)
{
	u8 index = RequestPtr % MAXBufferSize; // Calculate the index using modulo operation
	if(LCDRequest[index].State==Idle)
	{
		//Edit LCD Request Type and State
	    LCDRequest[index].Type=EnableCursor;
	    LCDRequest[index].State=Busy;
	}
    // Increment index and reset to 0 if it exceeds MAXBufferSize
	RequestPtr = (RequestPtr + 1) % MAXBufferSize;
}

// Runs every 2ms
void LCD_TASK(void)
{
	if(LCDMode==Init)
	{
		InitSM();
	}
	else if(LCDMode==Operation)
	{
		if(LCDRequest[CurrRunnablePtr].State==Busy)
		{
			switch(LCDRequest[CurrRunnablePtr].Type)
			{
			case WriteString:
			LCD_WriteStringProc();
			break;
			case WriteNumber:
			LCD_WriteNumberProc();
			break;
			case Clear:
			LCD_ClearProc();
			break;
			case SetPos:
			LCD_SetPosProc();
			break;
			case ShiftCursor:
			LCD_ShiftCursorProc();
			break;
			case EnableCursor:
			LCD_EnableCursorProc();
			break;
			case DisableCursor:
			LCD_DisableCursorProc();
			break;
			default:
			break;
			}
		}
	}
	else
	{
		//do nothing
	}
}

static void InitSM(void)
{
	static LCD_InitState_t LCD_InitState=PowerON;
	static u32 LocCounter_ms=0;
	switch (LCD_InitState)
	{
		case PowerON:
		LocCounter_ms+=LCD_Periodicityms;
		// wait 30 ms
		if((LocCounter_ms/30)>0)
		{
			LCD_InitState=FunctionSet;
		}
		break;
		case FunctionSet:
		switch(EnableState)
		{
		  case EN_High:
		  LCD_enuWriteCommand(LCD_FUNCSET_8BIT);
		  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
		  EnableState = EN_Low;
	      break;
		  case EN_Low:
		  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
		  EnableState = EN_High;
		  LCD_InitState=DisplayControl;
	      break;
		}
		break;

		case DisplayControl:
			switch(EnableState)
			{
			  case EN_High:
			  LCD_enuWriteCommand(LCD_DISPON_CURSON_BLON);
			  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
			  EnableState = EN_Low;
		      break;
			  case EN_Low:
			  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
			  EnableState = EN_High;
			  LCD_InitState=DisplayClear;
		      break;
			}
		break;

		case DisplayClear:
			switch(EnableState)
			{
			  case EN_High:
			  LCD_enuWriteCommand(LCD_CLR);
			  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
			  EnableState = EN_Low;
		      break;
			  case EN_Low:
			  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
			  EnableState = EN_High;
			  LCD_InitState=EntryModeSet;
		      break;
			}
		break;
		case EntryModeSet:
			switch(EnableState)
			{
			  case EN_High:
			  LCD_enuWriteCommand(LCD_ENTRYMODE_SETINC_SHOFF);
			  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
			  EnableState = EN_Low;
		      break;
			  case EN_Low:
			  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
			  EnableState = EN_High;
			  LCD_InitState=End;
		      break;
			}
		break;

		case End:

		LCDMode=Operation;

		if(fun)
		{
			fun();
		}
		break;
		default:
			// do nothing
		break;
	}
}

static void LCD_WriteStringProc(void)
{
	static EnableState_t EnableState=EN_High;
	if(WriteStrReq[CurrRunnablePtr].data[WriteStrReq[CurrRunnablePtr].CurrIdx]!='\0')
	{
		switch(EnableState)
		{
		  case EN_High:
		  LCD_enuWriteData(WriteStrReq[CurrRunnablePtr].data[WriteStrReq[CurrRunnablePtr].CurrIdx]);
		  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
		  EnableState = EN_Low;
	      break;
		  case EN_Low:
		  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
		  WriteStrReq[CurrRunnablePtr].CurrIdx++;
		  EnableState = EN_High;
	      break;
		}
	}
	else
	{
		LCDRequest[CurrRunnablePtr].State=Idle;
		LCDRequest[CurrRunnablePtr].Type=None;
		// Increment index with modulo operation to implement circular buffer behavior
		CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
		if(LCDRequest[CurrRunnablePtr].cb)
		{
			LCDRequest[CurrRunnablePtr].cb();
		}
	}
}
static void LCD_SetPosProc(void)
{
	static EnableState_t EnableState=EN_High;
	u8 Loc_u8DDRAM_ADD=0;
	 if(SetPosReq[CurrRunnablePtr].X_Pos==FirstLine)
	 {
		 Loc_u8DDRAM_ADD=SetPosReq[CurrRunnablePtr].Y_Pos;
	 }
	 else if(SetPosReq[CurrRunnablePtr].X_Pos==SecondLine)
	 {
		 Loc_u8DDRAM_ADD=SetPosReq[CurrRunnablePtr].Y_Pos+0x40;
	 }
	 else
	 {
		 //Do Nothing
	 }
	switch(EnableState)
	{
	  case EN_High:
	  /*set pointer to DDRAM*/
	  LCD_enuWriteCommand(Loc_u8DDRAM_ADD+128);
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
	  EnableState = EN_Low;
	  break;
	  case EN_Low:
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
	  EnableState = EN_High;
	  LCDRequest[CurrRunnablePtr].State=Idle;
	  LCDRequest[CurrRunnablePtr].Type=None;
	  // Increment index with modulo operation to implement circular buffer behavior
	  CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
	  if(LCDRequest[CurrRunnablePtr].cb)
	  {
		LCDRequest[CurrRunnablePtr].cb();
	  }
	  break;
	}
}

static void LCD_ClearProc(void)
{
	static EnableState_t EnableState=EN_High;
	switch(EnableState)
	{
	  case EN_High:
	  /* write CLR command*/
	  LCD_enuWriteCommand(LCD_CLR);
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
	  EnableState = EN_Low;
	  break;
	  case EN_Low:
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
	  EnableState = EN_High;
	  LCDRequest[CurrRunnablePtr].State=Idle;
	  LCDRequest[CurrRunnablePtr].Type=None;
	  // Increment index with modulo operation to implement circular buffer behavior
	  CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
	  if(LCDRequest[CurrRunnablePtr].cb)
	  {
		LCDRequest[CurrRunnablePtr].cb();
	  }
	  break;
	}
}
static void LCD_WriteNumberProc(void)
{
	static EnableState_t EnableState=EN_High;
	static u16 arr[32]={0};
	static u16 i=32;
	while(WriteNumReq[CurrRunnablePtr].Number!=0)
	{
		arr[i-1]=(WriteNumReq[CurrRunnablePtr].Number%10);
		WriteNumReq[CurrRunnablePtr].Number /= 10;
		i--;
	}
	if(i <= 31)
	{
		switch(EnableState)
		{
		  case EN_High:
		  LCD_enuWriteData(arr[i] + '0');
		  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
		  EnableState = EN_Low;
	      break;
		  case EN_Low:
		  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
		  i++;
		  EnableState = EN_High;
	      break;
		}
	}
	else
	{
		LCDRequest[CurrRunnablePtr].State=Idle;
		LCDRequest[CurrRunnablePtr].Type=None;
		// Increment index with modulo operation to implement circular buffer behavior
		CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
		if(LCDRequest[CurrRunnablePtr].cb)
		{
			LCDRequest[CurrRunnablePtr].cb();
		}
	}
}

void LCD_ShiftCursorProc(void)
{
	static EnableState_t EnableState=EN_High;
	switch(EnableState)
	{
	  case EN_High:
	  /* write shift cursor command*/
	  LCD_enuWriteCommand(ShiftCurReq[CurrRunnablePtr].Direction);
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
	  EnableState = EN_Low;
	  break;
	  case EN_Low:
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
	  EnableState = EN_High;
	  LCDRequest[CurrRunnablePtr].State=Idle;
	  LCDRequest[CurrRunnablePtr].Type=None;
	  // Increment index with modulo operation to implement circular buffer behavior
	  CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
	  if(LCDRequest[CurrRunnablePtr].cb)
	  {
		LCDRequest[CurrRunnablePtr].cb();
	  }
	  break;
	}
}

void LCD_DisableCursorProc(void)
{
	static EnableState_t EnableState=EN_High;
	switch(EnableState)
	{
	  case EN_High:
	  /* write Cursor OFF command*/
	  LCD_enuWriteCommand(LCD_DISPON_CURSOFF_BLOFF);
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
	  EnableState = EN_Low;
	  break;
	  case EN_Low:
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
	  EnableState = EN_High;
	  LCDRequest[CurrRunnablePtr].State=Idle;
	  LCDRequest[CurrRunnablePtr].Type=None;
	  // Increment index with modulo operation to implement circular buffer behavior
	  CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
	  if(LCDRequest[CurrRunnablePtr].cb)
	  {
		LCDRequest[CurrRunnablePtr].cb();
	  }
	  break;
	}
}

void LCD_EnableCursorProc(void)
{
	static EnableState_t EnableState=EN_High;
	switch(EnableState)
	{
	  case EN_High:
	  /* write Cursor ON command*/
	  LCD_enuWriteCommand(LCD_DISPON_CURSON_BLOFF);
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinHigh);
	  EnableState = EN_Low;
	  break;
	  case EN_Low:
	  GPIO_SetPinValue(LCD_PINS_CFG[LCD_E].PORT, LCD_PINS_CFG[LCD_E].PIN, PinLow);
	  EnableState = EN_High;
	  LCDRequest[CurrRunnablePtr].State=Idle;
	  LCDRequest[CurrRunnablePtr].Type=None;
	  // Increment index with modulo operation to implement circular buffer behavior
	  CurrRunnablePtr = (CurrRunnablePtr + 1) % MAXBufferSize;
	  if(LCDRequest[CurrRunnablePtr].cb)
	  {
		LCDRequest[CurrRunnablePtr].cb();
	  }
	  break;
	}
}

static void LCD_enuWriteCommand(u8 Copy_u8Command)
{
	 u8 Loc_u8LCDCounter=0;
	 u8 Loc_u8CommBit_iter=0;
	 /* RS=0: command mode*/
	 GPIO_SetPinValue(LCD_PINS_CFG[LCD_RS].PORT,LCD_PINS_CFG[LCD_RS].PIN,PinLow);
	 /*RW=0: Write mode*/
	 GPIO_SetPinValue(LCD_PINS_CFG[LCD_RW].PORT,LCD_PINS_CFG[LCD_RW].PIN,PinLow);
	 for(Loc_u8LCDCounter=LCD_DB0;Loc_u8LCDCounter<=LCD_DB7;Loc_u8LCDCounter++)
	 {
		 if(GET_BIT(Copy_u8Command,Loc_u8CommBit_iter))
		 {
			 GPIO_SetPinValue(LCD_PINS_CFG[Loc_u8LCDCounter].PORT,LCD_PINS_CFG[Loc_u8LCDCounter].PIN,PinHigh);
		 }
		 else
		 {
			 GPIO_SetPinValue(LCD_PINS_CFG[Loc_u8LCDCounter].PORT,LCD_PINS_CFG[Loc_u8LCDCounter].PIN,PinLow);
		 }
		 Loc_u8CommBit_iter++;
	 }
}

static void LCD_enuWriteData(u8 Copy_u8Data)
{
	 u8 Loc_u8LCDCounter=0;
	 u8 Loc_u8DataBit_iter=0;
	 /* RS=1: data mode*/
	 GPIO_SetPinValue(LCD_PINS_CFG[LCD_RS].PORT,LCD_PINS_CFG[LCD_RS].PIN,PinHigh);
	 /*RW=0: Write mode*/
	 GPIO_SetPinValue(LCD_PINS_CFG[LCD_RW].PORT,LCD_PINS_CFG[LCD_RW].PIN,PinLow);
	 for(Loc_u8LCDCounter=LCD_DB0;Loc_u8LCDCounter<=LCD_DB7;Loc_u8LCDCounter++)
	 {
		 if(GET_BIT(Copy_u8Data,Loc_u8DataBit_iter++))
		 {
			 GPIO_SetPinValue(LCD_PINS_CFG[Loc_u8LCDCounter].PORT,LCD_PINS_CFG[Loc_u8LCDCounter].PIN,PinHigh);
		 }
		 else
		 {
			 GPIO_SetPinValue(LCD_PINS_CFG[Loc_u8LCDCounter].PORT,LCD_PINS_CFG[Loc_u8LCDCounter].PIN,PinLow);
		 }
	 }
}

;
