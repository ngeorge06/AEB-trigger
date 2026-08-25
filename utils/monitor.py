""" Utility function for monitoring serial output and finding average response time """

#pip install pyserial
import serial
import numpy as np

PORT = 'COM4'  # either COM3 or COM4

ser = serial.Serial(PORT, 115200, timeout=1)  # open serial port

try:
    count = 0
    responses = []
    while True:
        line = ser.readline().decode('utf-8').strip()
        if line:
            responses.append(int(line.split(',')[-1]))  # store the last word in the line
            
except KeyboardInterrupt:
    print("----------- Calculating average ------------")
    if responses:
        print(f"Mean: {int(np.mean(responses))} µs")
        print(f"Median: {int(np.median(responses))} µs")
        print(f"Std Dev: {int(np.std(responses))} µs")
        print(f"Min: {int(np.min(responses))} µs")
        print(f"Max: {int(np.max(responses))} µs")
        print(f"N: {len(responses)}")
    else:
        print("No responses recorded.")


