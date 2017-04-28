/* 
  Example arduino code for relay nodes using universal proto file created 4-27-2017.
  This version only sends a message that is the size it needs to be to contain the data.
  Rename this .ino file as well as the folder it is in, they must match.
 */

#include "RAPID.pb.h"     // created from the RAPID.proto file
#include "pb_encode.h"
//#include "pb_decode.h"  // only needed if decoding messages
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
#define nodeID 31    // The node ID for each sensor node should be different


uint32_t displayTimer = 0;
int sensorValue = 0;
float voltage = 0;

// Protocol initializing, need to experiment with buffer size, currently using 64 byte
SensorData message = SensorData_init_zero;
uint8_t sensorDataBuffer[64];
pb_ostream_t stream={0};
size_t stream_size;



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

  // continually runs mesh.update() to keep network connected
  mesh.update();
  
  // Send to the master node every minute
  if (millis() - displayTimer >= 60000) {
    displayTimer = millis();

    // save message values to message struct
    message.sensorId=nodeID;       // sensorId is required
    // sends voltage read at A0, connect to Vin to read input voltage
    sensorValue = analogRead(A0);
    voltage= sensorValue * (5.0 / 1023.0);
    message.has_value = true;      // set message.has_value to true if sending "value"
    message.value=voltage;

    // encode the message to the buffer with the encode function
    encodeMessage();
    // send the message using mesh
    sendMessage();
  }
}

// send message function from mesh example, except it sends sensorDataBuffer instead
void sendMessage() {
  // Send an 'M' type message containing the encoded message
  if (!mesh.write(sensorDataBuffer, 'M', stream_size)) {
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
      Serial.print("Send OK: "); Serial.println(stream_size);
  }
}

// encoding function
void encodeMessage() {
  // encodes message struct to senosrDataBuffer
  stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
  pb_encode(&stream, SensorData_fields, &message);
  stream_size = stream.bytes_written;  // saves the number of bytes that contain the data
}

