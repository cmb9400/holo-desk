import socket
import sys
import struct

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)

server_address = ('localhost', 1313)
sock.bind(server_address)

sock.listen(1)

current_coord = 'x'
while True:
    connection, client_address = sock.accept()

    try:
        for i in range(0, 2):
            data = connection.recv(16)
            if data:
                int_data = int.from_bytes(data, byteorder = 'big')
                int_data *= 65535

                if current_coord == 'x':
                    int_data /= 1920
                    current_coord = 'y'
                elif current_coord == 'y':
                    int_data /= 1080
                    current_coord = 'x'

                print(str(int_data))
                # Do something with data
            else:
                break
    finally:
        connection.close()
