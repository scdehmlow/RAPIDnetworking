// Example arduino code for temperature reading node

#include "temperature_humidity.pb.h"
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
uint8_t sensorDataBuffer[32];
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
    
    // read temperature
    message.has_temperature = true;    // tells decoder that there is a temperature value
    message.temperature=analogRead(7); // replace analogRead(7); with temperature reading
    // read humidity
    message.has_humidity = true;       // tells decoder that there is a humidity value
    message.humidity=analogRead(7);    // replace analogRead(7); with humidity reading
    
    // encode the message to the buffer with the encode function
    encodeMessage();
    // send the message using mesh
    sendMessage();
  }
}

//------------FUNCTIONS------------------------
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

