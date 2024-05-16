// /*
//  * USART.h
//  *
//  * Created: 22/3/2024 5:53:43 PM
//  *  Author: Eman
//  */

// #ifndef USART_H_
// #define USART_H_

// /************************************************************************************
//  *                                       datatypes                                  *
//  * **********************************************************************************/
// #include "STD_LIB/std_types.h"
// typedef enum
// {
//     USART_Ok,
//     USART_WRONGCHANNEL,
//     USART_NULLPOINTER,
//     USART_wrongchannel,
//     USART_WRONGFREQ,
//     USART_TIMEOUT,
//     USART_BUSY
// } USART_ErrorStatus_t;

// typedef struct
// {
//     u32 Channel;
//     u32 Oversampling_mode;
//     u32 USART_enable;
//     u32 Word_length;
//     u32 Wakeup_method;
//     u32 Parity_control_enable;
//     u32 Parity_selection;
//     u32 PE_interrupt_enable;
//     u32 TXE_interrupt_enable;
//     u32 Transmission_complete_interrupt_enable;
//     u32 RXNE_interrupt_enable;
//     u32 IDLE_interrupt_enable;
//     u32 Transmitter_enable;
//     u32 Receiver_enable;
//     u32 Receiver_wakeup;
//     u32 Send_break;
//     u32 STOP_bits;
//     u16 baudrate;

// } USART_Config_t;

// typedef enum
// {
//     USART1_channel,
//     USART2_channel,
//     USART6_channel

// } USART_Channels_t;

// typedef struct
// {
//     u8 *data;
//     u32 pos;
//     u32 size;
// } USART_buffer_t;

// typedef void (*TxCB)(void);
// typedef void (*RxCB)(void);

// typedef struct
// {
//     volatile USART_buffer_t *buffer;
//     volatile u32 state;
//     volatile TxCB cb;
// } TXReq_t;

// typedef struct
// {
//     volatile USART_buffer_t *buffer;
//     volatile u32 state;
//     volatile RxCB cb;
// } RXReq_t;

// /************************************************************************************
//  *                                       #defines                                  *
//  * **********************************************************************************/

// #define F_CLK 16000000
// #define NUMBER_OF_USART 3
// #define NULL         ((void *) 0)

// #define USART_ENABLE 0x00002000
// #define USART_DISABLE 0xFFFFDFFF

// #define USART_OVERSAMPLING_16 0x00000000
// #define USART_OVERSAMPLING_8 0x00008000

// #define USART_DATA_BITS_8 0x00000000
// #define USART_DATA_BITS_9 0x00001000

// #define USART_PARITY_ON 0x00000400
// #define USART_PARITY_OFF 0xFFFFFBFF

// #define USART_PARITY_NONE 0x00000000
// #define USART_PARITY_ODD 0x00000200
// #define USART_PARITY_EVEN 0x00000000

// #define USART_TXE_INTERRUPT_ENABLE 0x00000080
// #define USART_TXE_INTERRUPT_DISABLE 0xFFFFFF7F

// #define USART_TX_COMPLETE_INT_ENABLE 0x00000040
// #define USART_TX_COMPLETE_INT_DISABLE 0x00000000

// #define USART_RXNE_INT_ENABLE 0x00000020
// #define USART_RXNE_INT_DISABLE 0xFFFFFFDF

// #define USART_STOP_BITS_HALF 0x00001000
// #define USART_STOP_BITS_ONE 0x00000000
// #define USART_STOP_BITS_ONE_AND_HALF 0x00003000
// #define USART_STOP_BITS_TWO 0x00002000

// #define USART_TX_ENABLE 0x00000008
// #define USART_RX_ENABLE 0x00000004

// #define ENABLE           1
// #define DISABLE          0

// #define USART_NOT_FINISH 0
// #define USART_FINISH 1

// // u8 USART1_Finish_Receiving = USART_NOT_FINISH;
// // u8 USART2_Finish_Receiving = USART_NOT_FINISH;
// // u8 USART6_Finish_Receiving = USART_NOT_FINISH;

// // typedef void(*CallBackfun)(void);
// /************************************************************************************
//  *                                       functions                                  *
//  * **********************************************************************************/
// /*
// USART_ErrorStatus_t USART_Init(const USART_Config_t* ConfigPtr);
// USART_ErrorStatus_t USART_SendByteAsynch(USART_Channels_t Channel, u8* buffer, u32 length, TxCB cb);
// USART_ErrorStatus_t USART_ReceiveByteAsynch(USART_Channels_t Channel, u8* buffer, u32 length, RxCB cb);
// USART_ErrorStatus_t USART_SendBufferZeroCopy(USART_Channels_t Channel, u8* buffer, u32 length);
// */
// USART_ErrorStatus_t USART_InitAsyn(USART_Config_t *ConfigPtr);
// USART_ErrorStatus_t USART_TxBufferAsyncZeroCopy(USART_Channels_t Channel, u8 *buffer, u16 len, TxCB cbf);
// USART_ErrorStatus_t USART_RxBufferAsyncZeroCopy(USART_Channels_t Channel, u8 *buffer, u16 len, RxCB cbf);
// USART_ErrorStatus_t USART_SendByte(USART_Channels_t Channel, u8 buffer);
// USART_ErrorStatus_t USART_ReceiveByte(USART_Channels_t Channel, u8* buffer);
// #endif /* USART_H_ */
#ifndef USART_H_
#define USART_H_

#include "STD_LIB/std_types.h"
#define F_USART1  16000000
#define F_USART2  16000000
#define F_USART6  16000000
/**************************************************************************/
/*					UART Channel Options      		    	 			  */
/**************************************************************************/
#define USART_CH1         (0U)
#define USART_CH2         (1U)
#define USART_CH6         (2U)

/**************************************************************************/
/*					UART Channel Oversampling Options           	      */
/**************************************************************************/
#define USART_OVERSAMPLING_16                0x00000000
#define USART_OVERSAMPLING_8                 0x00008000

/**************************************************************************/
/*					UART Channel Data Bits Options           	          */
/**************************************************************************/
#define USART_DATA_BITS_8                    0x00000000
#define USART_DATA_BITS_9                    0x00001000

/**************************************************************************/
/*					UART Channel Parity Bits Options           	          */
/**************************************************************************/
#define USART_PARITY_NONE                    0x00000000  
#define USART_PARITY_ODD                     0x00000200
#define USART_PARITY_EVEN                    0x00000000

/**************************************************************************/
/*					UART Stop Bits Options                  	          */
/**************************************************************************/
#define USART_STOP_BITS_HALF                 0x00001000
#define USART_STOP_BITS_ONE                  0x00000000
#define USART_STOP_BITS_ONE_AND_HALF         0x00003000
#define USART_STOP_BITS_TWO                  0x00002000

/**
  * @brief:  "fnpointer" type: pointer to function that takes void and returns void
  */
typedef void (*fnpointer)(void);

/**
 * @brief   Struct for Storing UART Post Compile Initilization Info
*/
typedef struct
{
    u8 Channel;             /**
                             * 1) USART_CH1    
                             * 2) USART_CH2   
                             * 3) USART_CH6  
                             */

    u8 OverSampling;        /**
                             * 1) USART_OVERSAMPLING_16
                             * 2) USART_OVERSAMPLING_8
                             */
    
    u8 WordLength;          /**
                             * 1) USART_DATA_BITS_8
                             * 2) USART_DATA_BITS_9
                             */
    
    u8 ParitySelect;        /**
                             * 1) USART_PARITY_NONE
                             * 2) USART_PARITY_ODD
                             * 3) USART_PARITY_EVEN
                             */

    u8 StopBits;            /**
                             * 1) USART_STOP_BITS_HALF
                             * 2) USART_STOP_BITS_ONE
                             * 3) USART_STOP_BITS_ONE_AND_HALF
                             * 4) USART_STOP_BITS_TWO
                             */

    f32 BaudRate;           /**
                             * @note Baud Rate Number Shouldn't Exceed:
                             *      1) F_UART/8 for Oversampling=8
                             *      2) F_UART/16 for Oversampling=16
                            */
}USART_PostCompileCfg_t;

/**
 * @brief:  UART Error Status Type
*/
typedef enum
{
    USART_Ok,
    USART_NullPtr,
    USART_Nok
}USART_ErrorStatus_t;


/**
 * @brief   Initializes a single USART peripheral 
 *
 * @param   - cfg: Pointer to Struct that stores configuration for a UART channel
 *
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_Init(USART_PostCompileCfg_t* cfg);

/**
 * @brief   - Takes a Buffer of Bytes to Transmit Asynchronously via a USART Peripheral
 *          - Jumps to USART ISR after every byte successfully sent
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) buffer: Pointer to a character/string to transmit asynchronously via USART
 * 
 *          - 3) len: Length of buffer
 * 
 *          - 4) cbf: Callback Function to call after transmission of buffer
 *                            
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_TxBufferAsyncZeroCopy(u8 USART_Num,char*buffer, u16 len, fnpointer cbf );

/**
 * @brief   - Takes a Buffer of Bytes to Store received Bytes (of size "len") Asynchronously via a USART Peripheral 
 *          - Jumps to USART ISR after every byte successfully received
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) buffer: Pointer to a character/string to store received buffer 
 * 
 *          - 3) len: Length of buffer to receive
 * 
 *          - 4) cbf: Callback Function to call after receiving of buffer
 *                            
 * @return  Error Status 
 */
USART_ErrorStatus_t USART_RxBufferAsyncZeroCopy(u8 USART_Num,u8*buffer, u16 len, fnpointer cbf);

/**
 * @brief   Sends a Byte over a USART Channel 
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) byte: Data to send
 * 
 * @return  Error Status: Returns if Data is successfully transmitted or not
 */
USART_ErrorStatus_t USART_SendByte(u8 USART_Num,u8 byte);

/**
 * @brief   Received a byte over a USART Channel 
 *
 * @param   - 1) USART_Num:
 *                     1) USART_CH1    
 *                     2) USART_CH2   
 *                     3) USART_CH6  
 * 
 *          - 2) byte: Pointer to variable to store received data
 * 
 * @return  Error Status: Returns if Data is successfully received or not
 */
USART_ErrorStatus_t USART_GetByte(u8 USART_Num,u8*byte);

//u8 USART_TxDone(void);
//u8 USART_IsRx(void);


#endif