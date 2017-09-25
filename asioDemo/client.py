#!/usr/bin/python3

import socket

cSocket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)

sHost = socket.gethostname()

nPort = 8088

cSocket.connect((sHost, nPort))

cSocket.send(b'hello, this is a python3 client')

sMsg = cSocket.recv(1024)

cSocket.close()

print(sMsg)
