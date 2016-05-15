#!/usr/bin/python3
#test program for receiving from the mega

import serial

receive = serial.Serial("/dev/ttyACM0", 9600)

while True:
	line = s.readline()
	msg = line.decode("ascii").strip()
	print(msg)
