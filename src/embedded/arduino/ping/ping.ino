#include <nRFModule.h>
#include <SPInterface.h>

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0x05, 0x04, 0x03, 0x02, 0x01};
uint8_t server_address[5]    = {0x01, 0x02, 0x03, 0x04, 0x05};

uint8_t rx[32];
uint8_t tx[32];

spi_interface 	spi = spi_interface();
NRF24L01    	nrf = NRF24L01(&spi);
WiseRFComm*     net = NULL;

uint32_t dummyData = 0;
uint32_t interval = 0;

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

void hardware_layer_data_arrived_handler () {
  printBuffer ("RX ", nrf.rx_buffer_ptr, 32);
  printBuffer ("TX ", nrf.tx_buffer_ptr, 32);
  
  net->parseRXRawData ();
}

void setup () {
  Serial.begin(9600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  net = new WiseRFComm(&nrf, tx, rx, hardware_layer_data_arrived_handler);
  net->setSender (local_address);
  net->setDataHandler (network_layer_data_arrived_handler);
  
  interval = millis ();
  Serial.println("TLM :: Initialized...");
}

void loop () {
  net->listenForIncoming ();
  
  if (abs(millis() - interval) > 1000) {
    dummyData++;
    
    net->clearBufferTX ();
    memcpy (nrf.tx_buffer_ptr, &dummyData, sizeof (dummyData));
    net->sendPacket (server_address);
    
    Serial.println ("SENT");
    interval = millis ();
  }
}
