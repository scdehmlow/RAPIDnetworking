 
 
/*
-----------------README-----------------
As with the previous version, you need to add this to the makefile in the RF24Mesh/example_RPi
* library. I obviously haven't tested this since I haven't been around the past few days.
* (sorry for that by the way). Hopefully this works. If not, I will be trying to locate
* someone to test with by tomorrow. If anyone tests this before then, let me know if it
* doesn't work and I will try to fix it before Tuesday at the latest.

*/
  
#include "RF24Mesh/RF24Mesh.h"  
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>


RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);  
RF24Network network(radio);
RF24Mesh mesh(radio,network);

uint32_t displayTimer=0;

int main(int argc, char** argv) {
  
  // Set the nodeID to 0 for the master node
  mesh.setNodeID(03);
  // Connect to the mesh
  printf("start\n");
  mesh.begin();
  radio.printDetails();

while(1)
{
  
  // Call network.update as usual to keep the network updated
  mesh.update();

  // In addition, keep the 'DHCP service' running on the master node so addresses will
  // be assigned to the sensor nodes
  mesh.DHCP();
  
  
  // Check for incoming data from the sensors
  while(network.available()){
//    printf("rcv\n");
    RF24NetworkHeader header;
    network.peek(header);
    
    uint32_t dat=0;
    switch(header.type){
      // Display the incoming millis() values from the sensor nodes
      case 'M': network.read(header,&dat,sizeof(dat)); 
                printf("Rcv %u from 0%o\n",dat,header.from_node);
                 break;
      default:  network.read(header,0,0); 
                printf("Rcv bad type %d from 0%o\n",header.type,header.from_node); 
                break;
    }
  }
delay(2);
if(!mesh.write(&displayTimer,'M',sizeof(displayTimer))){
	if(!mesh.checkConnection()){
	printf("Renewing Address\n");
	mesh.renewAddress();	
  }else{
	  printf("Send fail, Test OK\n");
}}else{
	printf("Send OK: %u\n",displayTimer);}
return 0;
}}

      
      
      
