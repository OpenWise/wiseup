#include <OneWire.h>
#include <nRFModule.h>
#include <SPInterface.h>

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0xFA, 0xFA, 0xFA, 0xFA, 0xFA};
uint8_t server_address[5]    = {0x05, 0x04, 0x03, 0x02, 0x01};

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);
rfcomm_data*   		packet 			= NULL;

void
printBuffer (char* name, uint8_t* buff, int len) {
    Serial.print (name);
    for (int i = 0; i < len; i++) {
        Serial.print (buff[i], HEX);
    } Serial.println ();
}

void hardware_layer_data_arrived_handler () {
  Serial.println ("hardware_layer_data_arrived_handler");
  packet = (rfcomm_data *)nrf.tx_buffer_ptr;
  uint8_t magic[] = {0xAA, 0xBB};
  // memcpy (packet->magic_number, magic, 2);
  
  printBuffer ("Sending to ", server_address, 5);
  nrf.send ();
}

void setup () {
  Serial.begin(57600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  nrf.setSourceAddress      ((byte *) local_address);
  nrf.setDestinationAddress ((byte *) server_address);
  nrf.setPayload (28);
  nrf.configure ();
  nrf.setSpeedRate (NRF_250KBPS);
  nrf.setChannel (1);	
  nrf.dataRecievedHandler = hardware_layer_data_arrived_handler;
  
  // nrf.printDetails ();

  Serial.println("TLM :: Initialized...");
}

void loop () {
  nrf.pollListener ();
}
