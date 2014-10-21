/*
 * EEPROM Read
 *
 * Prints EPPROM content
 *
 * Author: Roy Beiser
 */

#include <EEPROM.h>

// start reading from the first byte (address 0) of the EEPROM
int offset = 0;
byte value;
byte client_addr[5];

void setup()
{
  // initialize serial and wait for port to open:
  Serial.begin(9600);
}

void read_structures()
{
  byte type, len, temp;
  int sensor_type, sensor_address, sensor_pin, sensor_pin1, sensor_pin2;
  
  type = EEPROM.read(offset++);
  Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(type);
  len = EEPROM.read(offset++);  
  switch(type) {
      case 1: // Structure type 1 = client address
        Serial.println("\t\t Structure type 1 = client address");
          Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(len);Serial.println("\t\t Payload length");
        for (int i=0; i<len; i++) {
          client_addr[i] = EEPROM.read(offset++); 
          Serial.print(offset-1);
          Serial.print("\t\t");
          if (client_addr[i]<0x10) {Serial.print("0");}
          Serial.print(client_addr[i],HEX);  
          Serial.print("\t\t Address byte ");
          Serial.println(i+1);
        }
        break;
      case 2: // Structure type 2 = Sensor w/ 1 pin
        Serial.println("\t\t Structure type 2 = Sensor w/ 1 pin");
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(len);Serial.println("\t\t Payload length");
        sensor_type=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_type);Serial.println("\t\t Sensor type");        
        sensor_address=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_address);Serial.println("\t\t Sensor address");        
        sensor_pin=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_pin);Serial.println("\t\t Sensor pin");        
        break;
      case 3: // Structure type 3 = Sensor w/ 2 pins
        Serial.println("\t\t Structure type 3 = Sensor w/ 2 pins");
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(len);Serial.println("\t\t Payload length");
        sensor_type=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_type);Serial.println("\t\t Sensor type");        
        sensor_address=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_address);Serial.println("\t\t Sensor address");        
        sensor_pin1=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_pin1);Serial.println("\t\t Sensor pin 1");        
        sensor_pin2=EEPROM.read(offset++);
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(sensor_pin2);Serial.println("\t\t Sensor pin 2");        
        break;
      default: // type unknown
        Serial.println("\t\t Structure Unknown");
        Serial.print(offset-1);  Serial.print("\t\t");  Serial.print(len);Serial.println("\t\t Payload length");

        for (int i=0; i<len; i++) {
        temp = EEPROM.read(offset++); 
        }
        break;
  } 
      
  
  
  
}

void loop()
{
  // read a byte from the current address of the EEPROM
  int start_addr=0;
  int end_addr=20;
  Serial.print("Start addr =");
  Serial.println(start_addr,DEC);
  Serial.print("End   addr =");  
  Serial.println(end_addr,DEC);
  Serial.println();
  
  Serial.println("EPPROM addr    Value \t\t Description");
  for (int i=0; i<10;i++) {
    read_structures();
  }
  
   while (true);
}

