import socket
import sys
import struct
import serial

#to send to the leonardo
ser = serial.Serial('/dev/ttyUSB0', 115200, parity=serial.PARITY_EVEN)
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
    newX = int(32767 * (xIn / 1920))
    newY = int(32767 * (yIn / 1080))
    return (newX, newY)

def toWriteOutString(leoCoords):
    """
    Convert a tuple of two integers to a string to send
    to the Arduino Leonardo in the format:

      xxxxxyyyyy.

    where xxxxx is a right-aligned x coordinate precceded by 0's,
          yyyyy is a right-aligned y coordinate precceded by 0's,
    and a '.' terminates the string.
    """
    numZerosX = 5 - len(str(leoCoords[0]))
    numZerosY = 5 - len(str(leoCoords[1]))
    return ('0'*numZerosX)+str(leoCoords[0])+('0'*numZerosX)+str(leoCoords[1])+'.'

while True:
    kinectCoords = connection.readline()[:-1].split(' ')
    leoCoords = toLeoCoords(kinectCoords[0], kinectCoords[1])
    writeOutStr = toWriteOutString(leoCoords)
    ser.write((writeOutStr*2).encode('utf-8'))

connection.close()
