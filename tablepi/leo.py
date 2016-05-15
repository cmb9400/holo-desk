#test program for sending to the leonardo
import serial

ser = serial.Serial('/dev/ttyUSB0', 19200)
#print(ser.name) 
ser.write("3276732767.".encode('utf-8'))
print("done")

