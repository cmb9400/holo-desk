import socket
import sys
import struct
import serial

#to send to the leonardo
ser = serial.Serial('/dev/ttyUSB0', 115200)
ser.write("1000010000.".encode('utf-8'))

#networking horseshit
sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_address = ('0.0.0.0', 1313)
sock.bind(server_address)
sock.listen(1)
connection, client_address = sock.accept()
print(connection, client_address)
connection = connection.makefile()

def toLeoCoords(xIn, yIn):
    """
    Our input is expected to be in the ranges:
      xIn: [0, 1920]
      yIn: [0, 1080]

    This function will scale these values to the system
    of the Arduino Leonardo in the ranges:
      x: [0, 32767]
      y: [0, 32767]

    returned as a tuple: (x, y)
    """
    pass
    #newX = 

while True:
    kinectCoords = connection.readline()[:-1].split(' ')
    

connection.close()
