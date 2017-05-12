/** RF24Mesh_Example_Master.ino by TMRh20
  *
  * Note: This sketch only functions on -Arduino Due-
  *
  * This example sketch shows how to manually configure a node via RF24Mesh as $
  * will receive all data from sensor nodes.
  *
  * The nodes can change physical or logical position in the network, and recon$
  * routing nodes as required. The master node manages the address assignments $
  * in a manner similar to DHCP.
  *
  */

#include "RF24Mesh/RF24Mesh.h"
#include <RF24/RF24.h>
#include <RF24Network/RF24Network.h>

#include <stdio.h>
#include <stdlib.h>
#include <pb_encode.h>
#include <pb_decode.h>
#include "nanopb_RPI.pb.h"
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include <iostream>

using namespace std;


RF24 radio(RPI_V2_GPIO_P1_15, BCM2835_SPI_CS0, BCM2835_SPI_SPEED_8MHZ);
RF24Network network(radio);
RF24Mesh mesh(radio,network);

void printtime() {
    time_t rt;
    struct tm *info;
    char time_buffer[80];
    struct timeval tp;
    gettimeofday(&tp, NULL);
    rt = tp.tv_sec;
    info = gmtime(&rt);
    strftime(time_buffer,80,"%FT%T.",info);
    int ut = tp.tv_usec;
    printf("%s%.6i ",time_buffer,ut);
}

int main(int argc, char** argv) {

    // Set the nodeID to 0 for the master node
    mesh.setNodeID(0);
    // Connect to the mesh
    mesh.begin();
    mesh.setChannel();

    uint8_t buffer[64];
    size_t message_length=sizeof(buffer);
    SensorData message = SensorData_init_zero;
    pb_istream_t stream = pb_istream_from_buffer(buffer, message_length);

    while(1)
    {

        // Call network.update as usual to keep the network updated
        mesh.update();

        // In addition, keep the 'DHCP service' running on the master node so address
        // be assigned to the sensor nodes
        mesh.DHCP();


        // Check for incoming data from the sensors
        while(network.available()){
            printtime();
            RF24NetworkHeader header;
            network.peek(header);
            printf("%i NA %i %o ",radio.getChannel(), mesh.getNodeID(header.from_node), header.from_node);
            char TCPmsg[512]={};
            uint8_t dat[64];
            int m_length;
            m_length=network.read(header,&dat,message_length);
            printf(" Message length: %i ", m_length/sizeof(dat[0]));
            for (int i=0; i < m_length; i++)
            {
                printf("%02X",dat[i]);
            }
            cout<<"\n";

        }
        delay(2);
    }
    return 0;
}
