#!/usr/bin/env python3

# Exit codes
# !512 - Fail
# 512 - OK

import socket
from time import sleep
from datetime import datetime
import sys
import struct

if __name__ == "__main__":   
  msg = sys.argv[1]
else:
  exit(0)

def getUTC():
  return (datetime.utcnow().isoformat())
  
msg = getUTC() + " " + msg
  
# TCP IP/Port and Connect ####################################################################  
TCP_IP = 'winlink.from-il.com'
TCP_PORT = 22110
BUFFER_SIZE = 1024

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

# Connect & Send #############################################################################
s.send(msg.encode('ascii'))
r = s.recv(BUFFER_SIZE).decode('ascii')
print(r)
s.close()

if(r == "OK"):
  exit(1)
else:
  exit(0)