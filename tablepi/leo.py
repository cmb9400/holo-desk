#test program for sending to the leonardo

import serial
ser = serial.Serial('/dev/ttyUSB0', 9600)


while True:
    ser.write((input()).encode('utf-8'))
    print("done")
