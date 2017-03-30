#!/usr/bin/env python3

import socket
from time import sleep
from datetime import datetime
import sys
import struct

# Usage: python3 Send_TCP_Data.py msgType Date/Time nodeID numData [DATA 1] [DATA 2]
# - or - python3 Send_TCP_Data.py CHEK

if __name__ == "__main__":
  if(len(sys.argv) < 6):
    if(sys.argv[1] == "CHEK"):
      msgType = "CHEK"
    else:
      print("Usage: python3 Send_TCP_Data.py msgType Date/Time nodeID numData [DATA 1] [DATA 2]")
      print("- or - python3 Send_TCP_Data.py CHEK")
      sys.exit()
  else:
    msgType = sys.argv[1] + " "
    dateTime = sys.argv[2] + " "
    nodeID = sys.argv[3] + " "
    numData = sys.argv[4]
    DATA = []
    for i in range(5,5+int(numData)):
      DATA.append(sys.argv[i])

# Password Generator #########################################################################
def genPWD():
  cdt = datetime.utcnow().strftime("%Y %m %d %H %M %S").split(' ')
  Y = int(cdt[0])
  m = int(cdt[1])
  d = int(cdt[2])
  H = int(cdt[3])
  M = int(cdt[4])
  S = int(cdt[5])
  PASSWD = str(round(abs((S+1)*((Y*(m+M))/((m+H)-d)))))
  return PASSWD

# TCP IP/Port and Connect ####################################################################  
TCP_IP = '152.228.80.148'
TCP_PORT = 5005
BUFFER_SIZE = 1024

print("Attempting to connect...")

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((TCP_IP, TCP_PORT))

# Connect ####################################################################################
CONN = False
while CONN == False:
  s.send(str("CONN " + str(genPWD())).encode('ascii'))
  r = s.recv(BUFFER_SIZE)
  
  if(r.decode('ascii') == "CONN OK"):
    CONN = True
    print("Connected...")
  else:
    sleep(0.1)

# Send Data ##################################################################################
if(msgType != "CHEK"):
  msg = msgType + dateTime + nodeID
  for j in range(0,int(numData)):
    msg = msg + DATA[j] + " "
else:
  msg = msgType

s.send(msg.encode('ascii'))
r = s.recv(BUFFER_SIZE).decode('ascii')

if("OK" in r):
  print("Packet Sent.")
elif("IGN" in r):
  print("Packet Ignored by Server.")
else:
  print("Packet Failed to Send.")
  
# DISCONNECT #################################################################################
s.send("DISC".encode('ascii'))
r = s.recv(BUFFER_SIZE)

if(r.decode('ascii') == "DISC OK"):
  print("Disconnected...\n")
else:
  print("Failed to Disconnect!\n")
  
s.close()
sys.exit()