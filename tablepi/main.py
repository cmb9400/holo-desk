#!/usr/bin/python3

import serial

receive = serial.Serial("/dev/ttyACM0", 9600)
send = serial.SerialI("/dev/ttyUSB0",9600)

while True:
	line = s.readline()
	msg = line.decode("ascii").strip()
	send.write((msg + '\n').encode('utf-8'))
	print(msg)