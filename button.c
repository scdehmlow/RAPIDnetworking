/*
---------------README---------------
You can just copy paste this into the code that Shaffer uploaded to the github
You need all the libraries downloaded for it to work.
*/


#include <nRF24L01.h>
#include <printf.h>
#include <RF24.h>
#include <RF24_config.h>

/** RF24Mesh_Example.ino by TMRh20
 *
 * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
 * master node.
 * The nodes will refresh their network address as soon as a single write fails. This allows the
 * nodes to change position in relation to each other and the master node.
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
 * User Configuration: nodeID - A unique identifier for each radio. Allows addressing
 * to change dynamically with physical changes to the mesh.
 *
 * In this example, configuration takes place below, prior to uploading the sketch to the device
 * A unique value from 1-255 must be configured for each node.
 * This will be stored in EEPROM on AVR devices, so remains persistent between further uploads, loss of power, etc.
 *
 **/
#define nodeID 4

int BUTTON_PRESS=???; //I don't have arduino in front of me, so set pin number for this
uint32_t displayTimer = 0;

struct payload_t {
  unsigned long ms;
  unsigned long counter;
};

SensorData message = SensorData_init_zero;
uint8_t sensorDataBuffer[64];
pb_ostream_t stream={0};
static byte temperature = 0;
static byte humidity = 0;
static uint8_t counts=0;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(BUTTON_PRESS,INPUT);
  
  Serial.begin(115200);
  //printf_begin();
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}



void loop() {

  mesh.update();

  // Send to the master node every second
  //if (millis() - displayTimer >= 1000) {
    //displayTimer = millis();
    digitalWrite(LED_BUILTIN, LOW);
    if(digitalRead(BUTTON_PRESS)==HIGH){sendMessage();}
  }

//  while (network.available()) {
//    RF24NetworkHeader header;
//    payload_t payload;
//    network.read(header, &payload, sizeof(payload));
//    Serial.print("Received packet #");
//    Serial.print(payload.counter);
//    Serial.print(" at ");
//    Serial.println(payload.ms);
//  }
}

// nanopb send message function
void sendMessage() {
  message.has_value = true;
  message.value=analogRead(7);
  message.sensorId=nodeID;
  message.has_timestamp = true;
  message.timestamp=counts;
  counts = counts +1;
  stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
  pb_encode(&stream, SensorData_fields, &message);

 

  // Send an 'M' type message containing the current millis()
  if (!mesh.write(sensorDataBuffer, 'M', sizeof(sensorDataBuffer))) {

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
      Serial.print("Send OK: "); Serial.println((int)temperature);
      digitalWrite(LED_BUILTIN, HIGH);
  }
}