#!/usr/bin/env python3
import grpc
import gpio_service_pb2
import gpio_service_pb2_grpc
import unittest

############################ LED ON/OFF MACROS ####################

ON = 1
OFF = 0

############################ WRAPPER FUNCTIONS FOR STUBS ####################

def set_gpio_output( pin, state):
    response = stub.SetOutput(gpio_service_pb2.SetOutputRequest(pin=pin, state=state))
    return response.response

def set_gpio_mode( pin, mode):
    response = stub.ConfigurePin(gpio_service_pb2.ConfigurePinRequest(pin=pin, mode=mode))
    return response.response

def read_gpio_pin( pin):
    response = stub.ReadState(gpio_service_pb2.ReadStateRequest(pin=pin))
    return response.response

def initialize_connection():
    global stub
    # initialize connection with server IP over port:50051
    channel = grpc.insecure_channel('10.145.11.152:50051')
    stub = gpio_service_pb2_grpc.GPIOServiceStub(channel)



class TestGPIO(unittest.TestCase):
    
############################ SET PIN TEST CASES ####################

    def test_gpio_15_set_OFF(self):
        initialize_connection()
        self.assertEqual(set_gpio_output(15, OFF), True)

    def test_gpio_15_set_ON(self):
        initialize_connection()
        self.assertEqual(set_gpio_output(15, ON), True)

   
############################ GET PIN TEST CASES ####################

    def test_gpio_21_set_get_ON(self):
        initialize_connection()
        self.assertEqual(read_gpio_pin(21), ON)


    def test_gpio_21_get_OFF(self):
        initialize_connection()
        self.assertEqual(read_gpio_pin(21), OFF)



if __name__ == '__main__':
    initialize_connection()
    unittest.main()

