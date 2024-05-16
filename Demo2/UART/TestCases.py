<<<<<<< HEAD:Demo2/UART/TestCases.py
import serial
import struct

START_BYTE = 0xAA

# Define test case structure
class TestCase:
    def __init__(self, test_function, parameter):
        self.test_function = test_function
        self.parameter = parameter

# Function to calculate checksum
def calculate_checksum(payload_length, test_case, result):
    checksum = 0
    checksum ^= payload_length
    checksum ^= test_case.test_function
    checksum ^= test_case.parameter
    checksum ^= result
    return checksum & 0xFF  # Ensure the checksum is 8 bits

# Function to send test case and receive result
def send_and_receive_test_case(ser, test_case):
    payload_length = 2  # Length of test case data (2 bytes)
    result = 0x00  # Placeholder for result

    # Send test case frame
    ser.write(bytes([START_BYTE]))
    ser.write(bytes([payload_length]))
    ser.write(bytes([test_case.test_function, test_case.parameter]))
    checksum = calculate_checksum(payload_length, test_case, result)
    ser.write(bytes([checksum]))

    # Receive and process result frame
    start_byte = ser.read()
    if start_byte != bytes([START_BYTE]):
        print("Invalid start byte received.")
        return

    # Read payload length
    payload_length = ser.read()[0]

    # Read test function and parameter
    test_function, parameter = struct.unpack('BB', ser.read(2))

    # Read result
    result = ser.read()[0]

    # Read checksum
    received_checksum = ser.read()[0]

    # Calculate checksum
    checksum = calculate_checksum(payload_length, TestCase(test_function, parameter), result)

    # Check for checksum match
    if checksum != received_checksum:
        print("Checksum mismatch.")
        return

    # Process result
    if result == 0x01:
        print("Test case executed successfully.")
    elif result == 0x00:
        print("Test case execution failed.")
    else:
        print("Invalid result.")

# Example test cases
test_cases = [
    TestCase(0x01, 0x01),  # Test get_switch() with parameter 0x01
    TestCase(0x02, 0x02),  # Test Led_on() with parameter 0x02
    # Add more test cases as needed
]

# Serial port configuration
ser = serial.Serial('COM6', 9600, timeout=1)  # Set timeout to 1 second

try:
    # Send test cases and receive results
    for test_case in test_cases:
        send_and_receive_test_case(ser, test_case)

    # Check if no data is received
    if not ser.in_waiting:
        print("No data received from STM32.")

except serial.SerialException as e:
    print("Serial port error:", e)

finally:
    ser.close()
=======
# import serial
# import struct

# START_BYTE = 0xAA

# # Define test case structure
# class TestCase:
#     def __init__(self, test_function, parameter):
#         self.test_function = test_function
#         self.parameter = parameter

# # Function to calculate checksum
# def calculate_checksum(payload_length, test_case):
#     checksum = 0
#     checksum ^= payload_length
#     checksum ^= test_case.test_function
#     checksum ^= test_case.parameter
#     return checksum & 0xFF  # Ensure the checksum is 8 bits

# # Function to send test case
# def send_test_case(ser, test_case):
#     payload_length = 2  # Length of test case data (2 bytes)
#     checksum = calculate_checksum(payload_length, test_case)

#     ser.write(bytes([START_BYTE]))
#     ser.write(bytes([payload_length]))
#     ser.write(bytes([test_case.test_function, test_case.parameter]))
#     ser.write(bytes([checksum]))

# # Function to receive acknowledgment and result
# def receive_ack_and_result(ser):
#     ack = ser.read(1)
#     result = ser.read(2)
#     print(ack)
#     print(result)
#     return ack, result

# # Example test cases
# test_cases = [
#     TestCase(0x01, 0x01),  # Test get_switch() with parameter 0x01
#     TestCase(0x02, 0x02),  # Test Led_on() with parameter 0x02
#     # Add more test cases as needed
# ]

# # Serial port configuration
# ser = serial.Serial('COM6', 9600, timeout=1)  # Set timeout to 1 second

# try:
#     # Send test cases and receive acknowledgment and result
#     for test_case in test_cases:
#         send_test_case(ser, test_case)
#         print("Test case sent:", test_case.test_function, test_case.parameter)
        
#         ack, result = receive_ack_and_result(ser)
#         if ack == b'\x01':
#             print("Test case executed successfully")
#             print("Result:", result.decode())
#         else:
#             print("Test case execution failed")

#     # Check if no data is received
#     if not ser.in_waiting:
#         print("No data received from STM32.")

# except serial.SerialException as e:
#     print("Serial port error:", e)

# finally:
#     ser.close()
import serial

START_BYTE = 0xAA

# Serial port configuration
ser = serial.Serial('COM6', 9600, timeout=1)  # Set timeout to 1 second

# Function to receive test case result and acknowledgment
def receive_test_result():
    start_byte = ser.read()
    print(start_byte)
    if start_byte == bytes([START_BYTE]):
        
        payload_length = ser.read()[0]
        payload = ser.read(payload_length)
        checksum = ser.read()[0]

        # Calculate checksum
        calculated_checksum = (payload_length ^ payload[0] ^ payload[1]) & 0xFF

        # Check if checksum matches
        if checksum == calculated_checksum:
            # Check if result is "ok"
            
            if payload[2] == ord('o') and payload[3] == ord('k'):
                return True
    return False

# Example test cases
test_cases = [
    b'\xAA\x02\x01\x01\x03',  # Test case for function 0x01 with parameter 0x01
    b'\xAA\x02\x02\x02\x04'   # Test case for function 0x02 with parameter 0x02
    # Add more test cases as needed
]

try:
    # Send test cases
    for test_case in test_cases:
        ser.write(test_case)
        print("Test case sent:", test_case)

        # Wait for acknowledgment and result
        if receive_test_result():
            print("Test case executed successfully")
        else:
            print("Error: Failed to execute test case")

    # Check if no data is received
    if not ser.in_waiting:
        print("No data received from STM32.")

except serial.SerialException as e:
    print("Serial port error:", e)

finally:
    ser.close()
>>>>>>> f07665c7d1f25a6655fcc63c701ed0c097c8674e:TestCases.py
