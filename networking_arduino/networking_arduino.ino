/** RF24Mesh_Example.ino by TMRh20
 *
 * This example sketch shows how to manually configure a node via RF24Mesh, and send data to the
 * master node.
 * The nodes will refresh their network address as soon as a single write fails. This allows the
 * nodes to change position in relation to each other and the master node.
 */

#include "networking_arduino.pb.h"
#include "pb_encode.h"
//#include "pb_decode.h"
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>




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
#define nodeID 3    // The node ID for each sensor node should be different


uint32_t displayTimer = 0;
uint8_t counts=0;

// Protocol initializing, need to experiment with buffer size, currently using 64 byte
SensorData message = SensorData_init_zero;
uint8_t sensorDataBuffer[64];
pb_ostream_t stream={0};

;


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(115200);
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}



void loop() {

  mesh.update();

  // Send to the master node every second
  if (millis() - displayTimer >= 1000) {
    displayTimer = millis();

    // save message values to message struct
    message.sensorId=nodeID;       // sensorId is required
    // value is optional float type
    message.has_value = true;      // set message.has_value to true if sending "value"
    message.value=analogRead(7);
    // timestamp is optional int type
    message.has_timestamp = true;  // true if sending "timestamp"
    message.timestamp=counts;      // counts every time the message is sent for
    counts = counts +1;            // a rough timestamp

    // encode the message to the buffer with the encode function
    encodeMessage();
    // send the message using mesh
    sendMessage();
  }
}

// send message function from mesh example, except it sends sensorDataBuffer instead
void sendMessage() {
  // Send an 'M' type message containing the encoded message
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
      Serial.print("Send OK: "); Serial.println(message.sensorId);
      digitalWrite(LED_BUILTIN, HIGH);
  }
}

// encoding function
void encodeMessage() {
  // encodes message struct to senosrDataBuffer
  stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
  pb_encode(&stream, SensorData_fields, &message);
}

