<<<<<<< HEAD:Demo2/UART/main.c
 #include "APP/APP_Demo1/APP_Demo.h"

u8 START_BYTE = 0xAA;
// Structure to represent test cases
typedef struct {
    u8 test_function;
    u8 parameter;
} TestCase;

// Function declarations
void execute_test_case(TestCase *test_case);
void send_ack(u8 ack);
void receive_test_case();

void send_message(u8 *payload, u8 length);
void receive_message();
u8 buffer;

int main(void)
{
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
    RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOCEN,Periph_enuON);
	IPC_Init(USART_CH1);
    LED_Init();
	SWITCH_Init();
    
    while(1){
        receive_test_case();     
    }
	// SWITCH_Init();
	// LCD_InitPins();
	// LCD_init_asynch(NULLPTR);
	// scheduler_init();
	// APP_LCDMainMenu();
	// scheduler_start();
}


void receive_test_case() {
    u8 start_byte, payload_length, checksum, received_checksum;
    TestCase test_case;

    // Wait for start byte
    IPC_ReceiveUSART(USART_CH1, &start_byte, 1, NULLPTR);
    if (start_byte != START_BYTE) {
        return; // Discard message if start byte is incorrect
    }

    // Read payload length
    IPC_ReceiveUSART(USART_CH1, &payload_length, 1, NULLPTR);

    // Read test case data
    IPC_ReceiveUSART(USART_CH1, (u8 *)&test_case, 1, NULLPTR);

    // Calculate checksum
    IPC_ReceiveUSART(USART_CH1, &received_checksum, 1, NULLPTR);
    checksum = 0;
    checksum ^= payload_length;
    checksum ^= test_case.test_function;
    checksum ^= test_case.parameter;

    // Check for checksum match
    if (checksum == received_checksum) {
        // Execute test case
        execute_test_case(&test_case);
        //send report

    } else {
        // Error handling: Checksum mismatch
        send_ack(0); // Send NACK
    }
}
void execute_test_case(TestCase *test_case) {
  u8 Copy_Status = SWITCH_NOT_PRESSED;
  u8 state;
  u8 *payload;
    // Based on test_function, call corresponding test function
    switch (test_case->test_function) {
        case 0x01: // Test function for get_switch()
            // Call get_switch() with parameter test_case->parameter
            state = SWITCH_GetStatus(SWITCH_OK_MODE, &Copy_Status);
            if(Copy_Status == SWITCH_PRESSED||Copy_Status == SWITCH_NOT_PRESSED||state == SWITCH_OK){
                //passed
                payload = "ok";
                send_message(payload, 2);
            }
            else{
                //failed
            }
            IPC_SendUSART(USART_CH1, &Copy_Status, 1, NULLPTR);
            break;
        case 0x02: // Test function for Led_on()
            // Call Led_on() with parameter test_case->parameter
            state = LED_SetStatus(LED_RED, LED_STATE_ON);
            IPC_SendUSART(USART_CH1, &state, 1, NULLPTR);
            break;
        case 0x03:
            if(Copy_Status == SWITCH_PRESSED)
            {
              IPC_SendUSART(USART_CH1, &Copy_Status, 1, NULLPTR);
              state = LED_SetStatus(LED_RED, LED_STATE_ON);
              IPC_SendUSART(USART_CH1, &state, 1, NULLPTR);
            }
            else
            {
              IPC_SendUSART(USART_CH1, &Copy_Status, 1, NULLPTR);
              state = LED_SetStatus(LED_RED, LED_STATE_OFF);
              IPC_SendUSART(USART_CH1, &state, 1, NULLPTR);
            }
            break;
        default:
            // Invalid test function, send NACK
            send_ack(0);
            return;
    }
    // Test case executed successfully, send ACK
    send_ack(1);
    LED_SetStatus(LED_RED, LED_STATE_ON);
}
void send_ack(u8 ack) {
    IPC_SendUSART(USART_CH1, &ack, 1, NULLPTR);
}
void send_message(u8 *payload, u8 length) {
    u8 checksum = 0;

    // Send start byte
    IPC_SendUSART(USART_CH1, &START_BYTE, 1, NULLPTR);

    // Send payload length
    IPC_SendUSART(USART_CH1, &length, 1, NULLPTR);

    // Send payload
    for (u8 i = 0; i < length; i++) {
      IPC_SendUSART(USART_CH1, &payload[i], 1, NULLPTR);
        checksum ^= payload[i];
    }

    // Send checksum
    IPC_SendUSART(USART_CH1, &checksum, 1, NULLPTR);
}
void receive_message(u8* payload) {
    u8 start_byte, payload_length, checksum, received_checksum;
    //u8 payload[256]; // Max payload length assumed to be 256

    // Wait for start byte
    IPC_ReceiveUSART(USART_CH1, &start_byte, 1, NULLPTR);
    if (start_byte != START_BYTE) {
        return; // Discard message if start byte is incorrect
    }

    // Read payload length
    IPC_ReceiveUSART(USART_CH1, &payload_length, 1, NULLPTR);

    // Read payload
    for (u8 i = 0; i < payload_length; i++) {
      IPC_ReceiveUSART(USART_CH1, &payload[i], 1, NULLPTR);
    }

    // Calculate checksum
    IPC_ReceiveUSART(USART_CH1, &received_checksum, 1, NULLPTR);
    checksum = 0;
    for (u8 i = 0; i < payload_length; i++) {
        checksum ^= payload[i];
    }

    // Check for checksum match
    if (checksum == received_checksum) {
        //ok
    } else {
        // Error handling: Checksum mismatch
        payload = NULLPTR;
    }
}

// // Include necessary libraries
// //#include "main.h"
// #include "xcp.h" // Assuming XCP library is included
// #include "usart.h" // Assuming UART library is included

// // // Define GPIO pin for switch and LED
// // #define SWITCH_Pin GPIO_PIN_0
// // #define SWITCH_GPIO_Port GPIOA
// // #define LED_Pin GPIO_PIN_1
// // #define LED_GPIO_Port GPIOB

// // Initialize XCP communication
// XCP_StatusTypeDef xcp_status;

// void MX_XCP_Init(void) {
//   // Initialize XCP protocol
//   xcp_status = XCP_Init();
//   if (xcp_status != XCP_OK) {
//     Error_Handler(); // Handle initialization error
//   }
// }

// // // Initialize UART
// // void MX_UART_Init(void) {
// //   // Initialize UART peripheral
// //   //HAL_UART_MspInit(&huart1); // Assuming UART1 is used
// //   IPC_Init(USART_CH1);
// // }

// // // Initialize GPIOs
// // void MX_GPIO_Init(void) {
// //   GPIO_InitTypeDef GPIO_InitStruct = {0};

// //   // GPIO Ports Clock Enable
// //   __HAL_RCC_GPIOA_CLK_ENABLE();
// //   __HAL_RCC_GPIOB_CLK_ENABLE();

// //   // Configure GPIO pin for switch input
// //   GPIO_InitStruct.Pin = SWITCH_Pin;
// //   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
// //   GPIO_InitStruct.Pull = GPIO_PULLUP;
// //   HAL_GPIO_Init(SWITCH_GPIO_Port, &GPIO_InitStruct);

// //   // Configure GPIO pin for LED output
// //   GPIO_InitStruct.Pin = LED_Pin;
// //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
// //   GPIO_InitStruct.Pull = GPIO_NOPULL;
// //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
// //   HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
// // }
// int main(void) {
//   // Initialize MCU peripherals
// //   HAL_Init();
// //   SystemClock_Config();
// //   MX_GPIO_Init();
// //   MX_UART_Init(); // Initialize UART
// 	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
// 	IPC_Init(USART_CH1);
// 	SWITCH_Init();
//   	MX_XCP_Init(); // Initialize XCP communication
  
//   while (1) {
//     // Read switch state
//     // u8 switch_state = HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin);
    
//     // // Send switch state over UART
//     // HAL_UART_Transmit(&huart1, &switch_state, 1, HAL_MAX_DELAY);
// 	u8 switchState = SWITCH_NOT_PRESSED;
//     SWITCH_GetStatus(SWITCH_OK_MODE, &switchState);
// 	IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR);
//   }
// }

=======
// #include "APP/APP_Demo1/APP_Demo.h"

// u8 START_BYTE = 0xAA;

// // Structure to represent test cases
// typedef struct {
//     u8 test_function;
//     u8 parameter;
// } TestCase;

// // Function declarations
// void execute_test_case(TestCase *test_case);
// void send_ack(u8 ack);
// void receive_test_case();
// void send_message(u8 *payload, u8 length);
// void receive_message();
// u8 buffer;

// int main(void)
// {
//     RCC_enuEnableDisablePeripheral(RCC_AHB1, GPIOAEN, Periph_enuON);
//     RCC_enuEnableDisablePeripheral(RCC_AHB1, GPIOCEN, Periph_enuON);
//     IPC_Init(USART_CH1);
//     LED_Init();
//     SWITCH_Init();

//     while (1) {
//         receive_test_case();
// 		//send_message("  ok", 4);
//     }
// }

// void receive_test_case() {
//     u8 start_byte, payload_length, checksum, received_checksum;
//     TestCase test_case;

//     // Wait for start byte
//     IPC_ReceiveUSART(USART_CH1, &start_byte, 1, NULLPTR);
//     if (start_byte != START_BYTE) {
//         return; // Discard message if start byte is incorrect
//     }

//     // Read payload length
//     IPC_ReceiveUSART(USART_CH1, &payload_length, 1, NULLPTR);

//     // Read test case data
//     IPC_ReceiveUSART(USART_CH1, (u8 *)&test_case, 1, NULLPTR);

//     // Calculate checksum
//     IPC_ReceiveUSART(USART_CH1, &received_checksum, 1, NULLPTR);
//     checksum = 0;
//     checksum ^= payload_length;
//     checksum ^= test_case.test_function;
//     checksum ^= test_case.parameter;

//     // Check for checksum match
//     if (checksum == received_checksum) {
//         // Execute test case
//         execute_test_case(&test_case);
//     } else {
//         // Error handling: Checksum mismatch
//         send_ack(0); // Send NACK
//     }
// }

// void execute_test_case(TestCase *test_case) {
//     u8 Copy_Status = SWITCH_NOT_PRESSED;
//     u8 state;
//     u8 payload[4]; // Adjust payload size to accommodate result and ACK

//     // Based on test_function, call corresponding test function
//     switch (test_case->test_function) {
//         case 0x01: // Test function for get_switch()
//             state = SWITCH_GetStatus(SWITCH_OK_MODE, &Copy_Status);
//             if (state == SWITCH_OK || state == SWITCH_NOT_PRESSED) {
//                 payload[2] = 'o'; // 'o' for "ok"
//                 payload[3] = 'k';
//             } else {
//                 payload[2] = 'n'; // 'n' for "notok"
//                 payload[3] = 'o';
//             }
//             payload[0] = Copy_Status; // First byte: Copy_Status
//             payload[1] = state;        // Second byte: LED state
//             break;
//         case 0x02: // Test function for Led_on()
//             state = LED_SetStatus(LED_RED, LED_STATE_ON);
//             payload[0] = 0; // First byte: Placeholder for Copy_Status
//             payload[1] = state; // Second byte: LED state
//             payload[2] = 'o'; // 'o' for "ok"
//             payload[3] = 'k';
//             break;
//         case 0x03:
//             if (Copy_Status == SWITCH_PRESSED) {
//                 state = LED_SetStatus(LED_RED, LED_STATE_ON);
//             } else {
//                 state = LED_SetStatus(LED_RED, LED_STATE_OFF);
//             }
//             payload[0] = Copy_Status; // First byte: Copy_Status
//             payload[1] = state;        // Second byte: LED state
//             payload[2] = 'o'; // 'o' for "ok"
//             payload[3] = 'k';
//             break;
//         default:
//             // Invalid test function, send NACK
//             send_ack(0);
//             return;
//     }

//     // Send the result along with acknowledgment in the same frame
//     //IPC_SendUSART(USART_CH1, payload, 4, NULLPTR);

//     // Test case executed successfully, send ACK
//     //send_ack(1);
// 	send_message(payload, 4);
//     // Set LED status to ON
//     //LED_SetStatus(LED_RED, LED_STATE_ON);
// }


// void send_ack(u8 ack) {
//     IPC_SendUSART(USART_CH1, &ack, 1, NULLPTR);
// }

// void send_message(u8 *payload, u8 length) {
//     u8 checksum = 0;

//     // Send start byte
//     IPC_SendUSART(USART_CH1, &START_BYTE, 1, NULLPTR);

//     // Send payload length
//     IPC_SendUSART(USART_CH1, &length, 1, NULLPTR);

//     // Send payload
//     for (u8 i = 0; i < length; i++) {
//         IPC_SendUSART(USART_CH1, &payload[i], 1, NULLPTR);
//         checksum ^= payload[i];
//     }

//     // Send checksum
//     IPC_SendUSART(USART_CH1, &checksum, 1, NULLPTR);
// }

// // // Include necessary libraries
// // //#include "main.h"
// // #include "xcp.h" // Assuming XCP library is included
// // #include "usart.h" // Assuming UART library is included

// // // // Define GPIO pin for switch and LED
// // // #define SWITCH_Pin GPIO_PIN_0
// // // #define SWITCH_GPIO_Port GPIOA
// // // #define LED_Pin GPIO_PIN_1
// // // #define LED_GPIO_Port GPIOB

// // // Initialize XCP communication
// // XCP_StatusTypeDef xcp_status;

// // void MX_XCP_Init(void) {
// //   // Initialize XCP protocol
// //   xcp_status = XCP_Init();
// //   if (xcp_status != XCP_OK) {
// //     Error_Handler(); // Handle initialization error
// //   }
// // }

// // // // Initialize UART
// // // void MX_UART_Init(void) {
// // //   // Initialize UART peripheral
// // //   //HAL_UART_MspInit(&huart1); // Assuming UART1 is used
// // //   IPC_Init(USART_CH1);
// // // }

// // // // Initialize GPIOs
// // // void MX_GPIO_Init(void) {
// // //   GPIO_InitTypeDef GPIO_InitStruct = {0};

// // //   // GPIO Ports Clock Enable
// // //   __HAL_RCC_GPIOA_CLK_ENABLE();
// // //   __HAL_RCC_GPIOB_CLK_ENABLE();

// // //   // Configure GPIO pin for switch input
// // //   GPIO_InitStruct.Pin = SWITCH_Pin;
// // //   GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
// // //   GPIO_InitStruct.Pull = GPIO_PULLUP;
// // //   HAL_GPIO_Init(SWITCH_GPIO_Port, &GPIO_InitStruct);

// // //   // Configure GPIO pin for LED output
// // //   GPIO_InitStruct.Pin = LED_Pin;
// // //   GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
// // //   GPIO_InitStruct.Pull = GPIO_NOPULL;
// // //   GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
// // //   HAL_GPIO_Init(LED_GPIO_Port, &GPIO_InitStruct);
// // // }
// // int main(void) {
// //   // Initialize MCU peripherals
// // //   HAL_Init();
// // //   SystemClock_Config();
// // //   MX_GPIO_Init();
// // //   MX_UART_Init(); // Initialize UART
// // 	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
// // 	IPC_Init(USART_CH1);
// // 	SWITCH_Init();
// //   	MX_XCP_Init(); // Initialize XCP communication
  
// //   while (1) {
// //     // Read switch state
// //     // u8 switch_state = HAL_GPIO_ReadPin(SWITCH_GPIO_Port, SWITCH_Pin);
    
// //     // // Send switch state over UART
// //     // HAL_UART_Transmit(&huart1, &switch_state, 1, HAL_MAX_DELAY);
// // 	u8 switchState = SWITCH_NOT_PRESSED;
// //     SWITCH_GetStatus(SWITCH_OK_MODE, &switchState);
// // 	IPC_SendUSART(USART_CH1, &buffer, 1, NULLPTR);
// //   }
// // }

#include "APP/APP_Demo1/APP_Demo.h"
u8 state;u8 buff;
u8 Copy_Status = SWITCH_NOT_PRESSED;
u8 payload[4];
void cbf6(void){
	switch (buff) {
	case 0x01: // Test function for get_switch()
		state = SWITCH_GetStatus(SWITCH_OK_MODE, &Copy_Status);

		if (state == SWITCH_OK || state == SWITCH_NOT_PRESSED) {
			payload[2] = 'o'; // 'o' for "ok"
			payload[3] = 'k';
		} else {
			payload[2] = 'n'; // 'n' for "notok"
			payload[3] = 'o';
		}
		payload[0] = Copy_Status; // First byte: Copy_Status
		payload[1] = state;        // Second byte: LED state
		break;
	case 0x02: // Test function for Led_on()
		state = LED_SetStatus(LED_RED, LED_STATE_ON);
		payload[0] = 0; // First byte: Placeholder for Copy_Status
		payload[1] = state; // Second byte: LED state
		payload[2] = 'o'; // 'o' for "ok"
		payload[3] = 'k';
		break;
	case 0x03:
		if (Copy_Status == SWITCH_PRESSED) {
			state = LED_SetStatus(LED_RED, LED_STATE_ON);
		} else {
			state = LED_SetStatus(LED_RED, LED_STATE_OFF);
		}
		payload[0] = Copy_Status; // First byte: Copy_Status
		payload[1] = state;        // Second byte: LED state
		payload[2] = 'o'; // 'o' for "ok"
		payload[3] = 'k';
		break;
	default:
		// Invalid test function, send NACK
		//send_ack(0);
		break;
}
IPC_SendUSART(USART_CH1, &payload, 4, NULLPTR);
}

int main(void)
{
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOAEN,Periph_enuON);
	RCC_enuEnableDisablePeripheral(RCC_AHB1,GPIOCEN,Periph_enuON);
	IPC_Init(USART_CH1);
	SWITCH_Init();
	LED_Init();
	LCD_InitPins();
	LCD_init_asynch(NULLPTR);
	scheduler_init();
	u8 i=0;
	while (1)
	{
		IPC_ReceiveUSART(USART_CH1, &buff, 1, cbf6);
		//for(i=0;i<1000;i++);
	}
	
	// APP_LCDMainMenu();
	// scheduler_start();
}
>>>>>>> f07665c7d1f25a6655fcc63c701ed0c097c8674e:main.c
