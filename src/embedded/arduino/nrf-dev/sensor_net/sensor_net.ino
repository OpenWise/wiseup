#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0x05, 0x04, 0x03, 0x02, 0x01};
uint8_t server_address[5]    = {0xFA, 0xFA, 0xFA, 0xFA, 0xFA};

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);
WiseRFComm*             net                     = NULL;

rfcomm_data*   		packet 			= NULL;
uint32_t                sensor_info_timestamp     = 0;

void
printBuffer (char* name, uint8_t* buff, int len) {
    Serial.print (name);
    for (int i = 0; i < len; i++) {
        Serial.print (buff[i], HEX);
    } Serial.println ();
}

void
network_layer_broadcast_arrived_handler () {
  Serial.println ("network_layer_data_arrived_handler");
}

void
network_layer_data_arrived_handler () {
  Serial.println ("network_layer_data_arrived_handler");
}

void
hardware_layer_data_arrived_handler () {
  Serial.println ("hardware_layer_data_arrived_handler");
  net->parseRXRawData ();
}

void
setup () {
  Serial.begin(57600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  net = new WiseRFComm(&nrf, tx, rx, hardware_layer_data_arrived_handler);
  net->setSender (local_address);
  net->setDataHandler (network_layer_data_arrived_handler);
  net->setBroadcastHandler (network_layer_broadcast_arrived_handler);
  
  sensor_info_timestamp    = millis ();
  packet = (rfcomm_data *)nrf.tx_buffer_ptr;
  
  // nrf.printDetails ();
  
  Serial.println("TLM :: Initialized...");
}

void
loop () {
  net->listenForIncoming ();
  
  if (abs (millis () - sensor_info_timestamp) > 3000) {
    packet = (rfcomm_data *)nrf.tx_buffer_ptr;
    uint8_t magic[] = {0xAA, 0xBB};
    // memcpy (packet->magic_number, magic, 2);
    
    printBuffer ("Sending to ", server_address, 5);
    net->sendPacket (server_address);
    
    sensor_info_timestamp = millis ();
  }
}

