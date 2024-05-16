import serial

# Define test case structure
class TestCase:
    def __init__(self, test_function, parameter):
        self.test_function = test_function
        self.parameter = parameter

# Function to send test case
def send_test_case(ser, test_case):
    ser.write(bytes([test_case.test_function]))
    ser.write(bytes([test_case.parameter]))

# Function to receive acknowledgment and result
def receive_ack_and_result(ser):
    ack = ser.read(1)
    result = ser.read(2)
    result2 = ser.read(3)
    result3 = ser.read(4)
    print("Acknowledgment:", ack)
    print("Result:", result,result2) 
# Example test cases
test_cases = [
    TestCase(0x01, 0x01),  # Test get_switch() with parameter 0x01
    TestCase(0x02, 0x02),  # Test Led_on() with parameter 0x02
    # Add more test cases as needed
]

# Serial port configuration
ser = serial.Serial('COM6', 9600, timeout=1)  # Set timeout to 1 second

try:
    # Send test cases and receive acknowledgment and result
    for test_case in test_cases:
        send_test_case(ser, test_case)
        print("Test case sent:", test_case.test_function, test_case.parameter)
        
        receive_ack_and_result(ser)

    # Check if no data is received
    if not ser.in_waiting:
       print("Done.")

except serial.SerialException as e:
    print("Serial port error:", e)

finally:
    ser.close()
