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
//#include "pb_decode.h"
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
#define nodeID 1

int BUTTON_PRESS = 2; //Should be kept as pin 2 for the interrupt
int numNodes = 4;  //Number of nodes you will be sending to. Max is 4
int values[4];     //Should be the same as numNodes
int i = 0;

struct payload_t {
  unsigned long counter;
};

SensorData message = SensorData_init_zero;
uint8_t sensorDataBuffer[64];
pb_ostream_t stream = {0};


void setup() {
  pinMode(LED_BUILTIN, OUTPUT); //For test purposes
  pinMode(BUTTON_PRESS, INPUT);
  pinMode(3, INPUT);
  digitalWrite(LED_BUILTIN, LOW);

  Serial.begin(115200);
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println("Connecting to the mesh...");
  mesh.begin(99);
}



void loop() {

  mesh.update();
  attachInterrupt(digitalPinToInterrupt(BUTTON_PRESS), sendMessage, FALLING );
}

// nanopb send message function
void sendMessage() {
  //Serial.println("interrupt1");
  for (i = 0; i < numNodes; i++)
  {
    message.has_value = true; //Working out input/RF24 issues. First 4 inputs will read from D3-D6
    message.value = digitalRead(i + 3);
    Serial.println(message.value);

    message.sensorId = nodeID;
    message.has_timestamp = false;
    stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
    pb_encode(&stream, SensorData_fields, &message);



    // Send an 'M' type message containing the current millis()
    if (!mesh.write(sensorDataBuffer, 'M', sizeof(sensorDataBuffer), i + 2)) {

      // If a write fails, check connectivity to the mesh network
      if ( ! mesh.checkConnection() ) {
        //refresh the network address
        Serial.println("Renewing Address");
        mesh.renewAddress();
      } else {
        Serial.println("Send fail, Test OK");
      }
    } else {
      String strindat = (char*)sensorDataBuffer;
      Serial.print("Send OK: ");
      digitalWrite(LED_BUILTIN, HIGH);
    }

    delay(500);
  }
}

