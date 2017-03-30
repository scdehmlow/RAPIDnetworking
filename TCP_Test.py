import os
import sys
from datetime import datetime
from time import sleep

DATA = []

# Send Data Function (for master nodes)
def sendPacket(dataType, nodeID, numData, data=[]):
  if(dataType != "CHEK"):
    Time = str(datetime.utcnow().isoformat())
    pkt = ("python3 Send_TCP_Data.py " + dataType + " " + Time + " " + str(nodeID) + " " + str(numData))
    for j in range(0,int(numData)):
      pkt = (pkt + " " + data[j])
  else:
    pkt = ("python3 Send_TCP_Data.py CHEK")
  
  #print(pkt)
  os.system(str(pkt))

# User Input
dT = input("Data Type (DATA, STAT,  CHEK): ")
nID = input("nodeID (0-100): ")
nD = 0

if(dT != "CHEK"):
  nD = input("Number of Data Values: ")
  for i in range(0, int(nD)):
    DATA.append(input("Data " + str(i) + " '[name]=[value]': "))

sendPacket(dT, nID, nD, DATA)

sys.exit()