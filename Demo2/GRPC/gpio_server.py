import grpc  # Import gRPC library for communication
import gpio_service_pb2  # Import Protobuf messages
import gpio_service_pb2_grpc  # Import gRPC service definition
import RPi.GPIO as GPIO  # Import Raspberry Pi GPIO library
from concurrent import futures  # Import futures for asynchronous execution

class GPIOServiceServicer(gpio_service_pb2_grpc.GPIOServiceServicer):
    def ReadState(self, request, context): # Read state of GPIO pin
        print("Recieved read state request from client")  # Print message when request received
        pin = request.pin  # Extract pin number from request
        GPIO.setmode(GPIO.BCM)  # Set GPIO pin numbering mode
        GPIO.setup(pin, GPIO.IN)  # Set pin as input
        state = GPIO.input(pin)  # Read pin state
        return gpio_service_pb2.ReadStateResponse(response=state)  # Return response with pin state

    def SetOutput(self, request, context):
        # Set output state of GPIO pin
        print("Recieved set output request from client")  # Print message when request received
        pin = request.pin  # Extract pin number from request
        GPIO.setmode(GPIO.BCM)  # Set GPIO pin numbering mode
        GPIO.setup(pin, GPIO.OUT)  # Set pin as output
        GPIO.output(pin, request.state)  # Set pin output state
        return gpio_service_pb2.SetOutputResponse(response=True)  # Return response

    def ConfigurePin(self, request, context):
        # Configure pin mode
        print("Recieved pin configure request from client")  # Print message when request received
        pin = request.pin  # Extract pin number from request
        GPIO.setmode(GPIO.BCM)  # Set GPIO pin numbering mode
        if request.mode == gpio_service_pb2.ConfigurePinRequest.INPUT:
            GPIO.setup(pin, GPIO.IN, pull_up_down=GPIO.PUD_DOWN)  # Set pin as input with pull-down resistor
        elif request.mode == gpio_service_pb2.ConfigurePinRequest.OUTPUT:
            GPIO.setup(pin, GPIO.OUT)  # Set pin as output
        return gpio_service_pb2.ConfigurePinResponse(response=True)  # Return response

def serve():
    server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))  # Create gRPC server with thread pool
    gpio_service_pb2_grpc.add_GPIOServiceServicer_to_server(GPIOServiceServicer(), server)  # Add servicer to server
    server.add_insecure_port("[::]:50051")  # Add insecure port to listen on
    print("Started gpio server on port 50051")  # Print message when server starts
    server.start()  # Start server
    server.wait_for_termination()  # Wait for server termination

if __name__ == '__main__':
    GPIO.setwarnings(False)  # Disable GPIO warnings
    serve()  # Start server

