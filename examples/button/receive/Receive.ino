/*
  ---------------README---------------
  You can just copy paste this into the code that Shaffer uploaded to the github
  You need all the libraries downloaded for it to work.
*/

/** RF24Mesh_Example.ino by TMRh20

   This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
   master node.
   The nodes will refresh their network address as soon as a single write fails. This allows the
   nodes to change position in relation to each other and the master node.
*/

//#include "Ethernet.h"
#include "shaffer.pb.h"
#include "pb_encode.h"
#include "pb_decode.h"
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>


//#include <printf.h>



/**** Configure the nrf24l01 CE and CS pins ****/
RF24 radio(7, 8);
RF24Network network(radio);
RF24Mesh mesh(radio, network);

/**
   User Configuration: nodeID - A unique identifier for each radio. Allows addressing
   to change dynamically with physical changes to the mesh.

   In this example, configuration takes place below, prior to uploading the sketch to the device
   A unique value from 1-255 must be configured for each node.
   This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.

 **/
#define nodeID 2

int LED = 10;

struct payload_t {
  unsigned long counter;
};

SensorData message = SensorData_init_zero;
uint8_t sensorDataBuffer[64];
pb_ostream_t stream = {0};


void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin(99);
  uint8_t buffer[64];
  size_t message_length=sizeof(buffer);
  bool status;

  SensorData message = SensorData_init_zero;
  pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

}



void loop() {

  mesh.update();
  digitalWrite(LED_BUILTIN, HIGH);

  while (network.available()) {

    RF24NetworkHeader header;
    payload_t payload;
    network.read(header, &payload, sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" at ");

  }
}



