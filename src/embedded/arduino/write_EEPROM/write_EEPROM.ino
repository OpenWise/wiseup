/*
 *
 * Write info into the Arduino EEPROM
 *
 * Author: Roy Beiser
 *
 */


#include <EEPROM.h>

// the current address in the EEPROM (i.e. which byte
// we're going to write to next)
int offset = 0;

void write_address(byte client_addr[5]) 
{
        // Structure type 1 = client address
        Serial.print(offset);  Serial.print("\t\t"); Serial.print(1,HEX); Serial.println("\t\t Structure type 1 = client address");
	EEPROM.write(offset++, 1);  
        // Structure payload length = 5 Bytes
        Serial.print(offset); Serial.print("\t\t"); Serial.print(5,HEX); Serial.println("\t\t Structure payload length = 5 Bytes");
	EEPROM.write(offset++, 5);  
	for (int i=0; i<=4; i++)  {
		Serial.print(offset);
		Serial.print("\t\t");        
		if (client_addr[i]<0x10) {Serial.print("0");} 
		Serial.print(client_addr[i],HEX);  
                Serial.print("\t\t Address byte ");
                Serial.println(i+1);
		EEPROM.write(offset++, client_addr[i]);
    }
}

void write_sensor1pin(int sensor_type, int sensor_address, int sensor_pin)
{
        // Structure type 2 = Sensor w/ 1 pin
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(2,HEX);  Serial.println("\t\t Structure type 2 = Sensor w/ 1 pin");
	EEPROM.write(offset++, 2);  

        // Structure payload length = 3 Bytes
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(3,HEX); Serial.println("\t\t Structure payload length = 3 Bytes");
	EEPROM.write(offset++, 3);  

        // Sensor Type
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_type,HEX); Serial.println("\t\t Sensor Type");
      	EEPROM.write(offset++, sensor_type);  

        // Sensor address
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_address,HEX); Serial.println("\t\t Sensor address");
      	EEPROM.write(offset++, sensor_address);  
        
        // Sensor pin
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_pin); Serial.println("\t\t Sensor pin");
      	EEPROM.write(offset++, sensor_pin);  
}


void write_sensor2pins(int sensor_type, int sensor_address, int sensor_pin1, int sensor_pin2)
{
        // Structure type 3 = Sensor w/ 2 pins
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(3,HEX);  Serial.println("\t\t Structure type 3 = Sensor w/ 2 pins");
	EEPROM.write(offset++, 3);  

        // Structure payload length = 4 Bytes
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(3,HEX); Serial.println("\t\t Structure payload length = 3 Bytes");
	EEPROM.write(offset++, 4);  

        // Sensor Type
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_type,HEX); Serial.println("\t\t Sensor Type");
      	EEPROM.write(offset++, sensor_type);  

        // Sensor address
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_address,HEX); Serial.println("\t\t Sensor address");
      	EEPROM.write(offset++, sensor_address);  
        
        // Sensor pin 1
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_pin1); Serial.println("\t\t Sensor pin 1");
      	EEPROM.write(offset++, sensor_pin1);  

        // Sensor pin 2
        Serial.print(offset);  Serial.print("\t\t");  Serial.print(sensor_pin2); Serial.println("\t\t Sensor pin 2");
      	EEPROM.write(offset++, sensor_pin2);  
}


void setup()
{
	Serial.begin(9600);
	Serial.println("EPPROM addr    Value \t\t Description");
	byte client_addr[5] = { 0xE7, 0x0E, 0xE0, 0xE7, 0x01 };
	write_address(client_addr);

        //write_sensor1pin(int sensor_type, int sensor_address, int sensor_pin)
        write_sensor1pin(1,1,14);
        //write_sensor2pins(int sensor_type, int sensor_address, int sensor_pin1, int sensor_pin2)
        write_sensor2pins(4,2,15,16);

        Serial.println("\nEnd of write EEPROM");
        
}

void loop()
{

}


