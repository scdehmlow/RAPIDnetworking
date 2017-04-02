from RF24 import *
from RF24Network import *
from RF24Mesh import *
import nanopb_RPI_pb2
import struct
import binascii

# radio setup 
radio = RF24(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ)
network = RF24Network(radio)
mesh = RF24Mesh(radio, network)

mesh.setNodeID(0)
mesh.begin()
#radio.setPALevel(RF24_PA_MAX) # Power Amplifier
sensordata = nanopb_RPI_pb2.SensorData()
radio.printDetails()

while 1:
    mesh.update()
    mesh.DHCP()
    while network.available():
        print("Received message")
        header, payload = network.read(64+8)
        print(binascii.hexlify(payload))
        print(len(payload))
        #nanopb_RPI_pb2.SensorData.ParseFromStringnascii.hexlify(payload)u(,payload)
        #print(struct.unpack('<32B',payload))
        nanopb_RPI_pb2.SensorData.ParseFromString(sensordata,payload)
        #nanopb_RPI_pb2.SensorData.ParseFromString(sensordata,struct.unpack('<32B',payload))
        print(str(sensordata.value()))
