""" Utility function for monitoring serial output and finding average response time """

#pip install pyserial
import serial

PORT = 'COM3'  # either COM3 or COM4

ser = serial.Serial(PORT, 115200, timeout=1)  # open serial port

while True:
    line = ser.readline().decode('utf-8').rstrip()
    if line:
        print(line)
