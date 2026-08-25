""" Utility function for monitoring serial output and finding average response time """

#pip install pyserial
import serial

PORT = 'COM4'  # either COM3 or COM4

ser = serial.Serial(PORT, 115200, timeout=1)  # open serial port

try:
    count = 0
    responses = []
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line:
            print(line)
            print(line.split(',')[-1])  # print the last word in the line
            
except KeyboardInterrupt:
    print("----------- Calculating average ------------")


