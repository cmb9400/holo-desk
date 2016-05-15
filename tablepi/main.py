#!/usr/bin/python3

import serial

send = serial.Serial("/dev/ttyUSB0",9600)

while True:
	line = input()
	send.write((line + '\n').encode('utf-8'))
