# RAPIDnetworking

To use these examples you must download the entire github repository and extract the contents. Download zip on github and then unzip.

## Arduino Examples
Arduino examples can be found in the **examples_arduino** folder. Inside this folder the **temperature** folder contains the arduino files for a node that only has a temperature measurement. Open **temperature.ino** to open this example in the Arduino IDE. The **temperature_humidity** folder contains files for a node that will read temperature and humidity. Finally, the **bme280** folder contains files for the bme280 sensor that reads temperature, humidity, pressure, and altitude.
These examples send a message every second containing data. Values from an analogRead of pin 7 is used for each measurement as an example. Replace this reading the the corresponding sensor value and the code should work.

*NOTE:* RF24 libraries should be installed in the Arduino IDE including RF24, RF24Network, and RF24Mesh.

## Raspberry Pi Example
Raspberry Pi examples are located in the examples folder. Inside this folder the RF24_pb_no_server contains cpp files that can be run with a raspberry pi. To use this program run ```make``` in the directory and then you can run ```sudo ./RF24Mesh_Example_Master``` to print the data in the terminal or ```./RF24Mesh_Example_Master >> log.txt``` to log it instead.
