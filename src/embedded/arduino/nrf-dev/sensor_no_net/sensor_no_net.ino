#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0x05, 0x04, 0x03, 0x02, 0x01};
uint8_t server_address[5]    = {0x01, 0x01, 0x01, 0x01, 0x01};

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);

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
network_layer_data_arrived_handler () {
  Serial.println ("network_layer_data_arrived_handler");
}

void
setup () {
  Serial.begin(57600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  nrf.setSourceAddress      ((byte *) local_address);
  nrf.setDestinationAddress ((byte *) server_address);
  nrf.rx_buffer_ptr = rx;
  nrf.tx_buffer_ptr = tx;
  nrf.setPayload (32);
  nrf.setSpeedRate (NRF_250KBPS);
  nrf.configure ();
  nrf.dataRecievedHandler = network_layer_data_arrived_handler;
  
  sensor_info_timestamp    = millis ();
  packet = (rfcomm_data *)nrf.tx_buffer_ptr;
  
  // nrf.printDetails ();
  
  Serial.println("TLM :: Initialized...");
}

void
loop () {
  nrf.pollListener ();
  
  if (abs (millis () - sensor_info_timestamp) > 3000) {
    packet = (rfcomm_data *)nrf.tx_buffer_ptr;
    memcpy (nrf.tx_buffer_ptr, 0x0, 32);
    
    uint8_t magic[] = {0xAA, 0xBB};
    memcpy (&(packet->magic_number), magic, 2);
    
    printBuffer ("Sending to ", server_address, 5);
    nrf.send ();
    
    sensor_info_timestamp = millis ();
  }
}

