#include <nRFModule.h>
#include <SPInterface.h>

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0x01, 0x02, 0x03, 0x04, 0x05};

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);
WiseRfcomm              net                     = NULL;

uint32_t dummyData = 1;

void
printBuffer (char* name, uint8_t* buff, int len) {
    Serial.print (name);
    for (int i = 0; i < len; i++) {
        Serial.print (buff[i], HEX);
    } Serial.println ();
}

void
hardware_layer_data_arrived_handler () {
  dummyData++;
  //printBuffer ("RX ", nrf.rx_buffer_ptr, 32);
  
  // memset (nrf.tx_buffer_ptr, 0x0, 32);
  // memcpy (nrf.tx_buffer_ptr, &dummyData, sizeof (dummyData));
  // nrf.setDestinationAddress ((uint8_t *) server_address);
  // nrf.send ();
  
  // printBuffer ("TX ", nrf.tx_buffer_ptr, 32);
  Serial.println ();
}

void
setup () {
  Serial.begin(9600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  net = WiseRfcomm (&nrf);
  net.setHandler (hardware_layer_data_arrived_handler);

  Serial.println("TLM :: Initialized...");
}

void
loop () {
  nrf.pollListener ();
  delay (10);
}
