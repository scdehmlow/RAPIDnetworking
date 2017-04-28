/* 
  Example arduino code for all sensor nodes using universal proto file created 4-27-2017.
  This version only sends a message that is the size it needs to be to contain the data.
  Rename this .ino file as well as the folder it is in, they must match.
 */

// Optional sleep library, needs to be downloaded from library manager
#include <Sleep_n0m1.h>
// note that when a node is sleeping it can not relay messages
// see the relay example if you don't want to sleep

#include "RAPID.pb.h"     // created from the RAPID.proto file
#include "pb_encode.h"
//#include "pb_decode.h"  // only needed if decoding messages
#include "RF24.h"
#include "RF24Network.h"
#include "RF24Mesh.h"
#include <SPI.h>
#include <EEPROM.h>

// for sleeping
Sleep sleep;



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
#define nodeID 31    // The node ID for each sensor node should be different

// Protocol initializing
SensorData message = SensorData_init_zero;  // initializes message structure
uint8_t sensorDataBuffer[512];  // uses 512 byte buffer but ends up sending less
pb_ostream_t stream = {0};
size_t stream_size;  // used for the size of the encoded stream



void setup() {
  pinMode(LED_BUILTIN, OUTPUT);  // LED on when sending is optional

  Serial.begin(115200);
  // Set the nodeID manually
  mesh.setNodeID(nodeID);
  // Connect to the mesh
  Serial.println(F("Connecting to the mesh..."));
  mesh.begin();
}



void loop() {

  mesh.update();

  // put the arduino to sleep for about 60s
  sleep.sleepDelay(60000);

  // save message values to message struct
  message.sensorId = nodeID;     // sensorId is required

  // read value (any float like Vin voltage reading), delete this block if no value data
  message.has_temperature = true;    // tells decoder that there is a value
  message.value=analogRead(7); // replace analogRead(7); with value reading
  
  // read temperature, delete this block if no temperature data
  message.has_temperature = true;    // tells decoder that there is a temperature value
  message.temperature=analogRead(7); // replace analogRead(7); with temperature reading
  
  // read humidity, delete this block if no humidity data
  message.has_humidity = true;       // tells decoder that there is a humidity value
  message.humidity=analogRead(7);    // replace analogRead(7); with humidity reading

  // read pressure, delete this block if no pressure data
  message.has_pressure = true;       // tells decoder that there is a pressure value
  message.pressure=analogRead(7);    // replace analogRead(7); with pressure reading

  // read altitude, delete this block if no altitude data
  message.has_altitude= true;        // tells decoder that there is a altitude value
  message.altitude=analogRead(7);    // replace analogRead(7); with altitude reading
  
  // encode the message to the buffer with the encode function
  encodeMessage();
  // send the message using mesh
  sendMessage();
}

//------------FUNCTIONS------------------------
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
    Serial.print("Send OK: "); Serial.print(stream_size);
    Serial.println(" Bytes");
    digitalWrite(LED_BUILTIN, HIGH);
  }
}

// encoding function
void encodeMessage() {
  // encodes message struct to senosrDataBuffer
  stream = pb_ostream_from_buffer(sensorDataBuffer, sizeof(sensorDataBuffer));
  pb_encode(&stream, SensorData_fields, &message);
  stream_size = stream.bytes_written;  // saves the number of bytes that contain the data
}

