#include <OneWire.h>
#include <nRFModule.h>
#include <SPInterface.h>

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0x01, 0x02, 0x03, 0x04, 0x05};

uint8_t rx[32];
uint8_t tx[32];
uint8_t stage = 0;

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);
WiseRFComm*             net                     = NULL;

rfcomm_data*   		packetTX 		= NULL;
rfcomm_data*   		packetRX 		= NULL;

void
printBuffer (char* name, uint8_t* buff, int len) {
    Serial.print (name);
    for (int i = 0; i < len; i++) {
        Serial.print (buff[i], HEX);
    } Serial.println ();
}

void
network_layer_broadcast_arrived_handler () {
  Serial.println ("network_layer_broadcast_arrived_handler");
  net->clearBufferTX ();
  packetTX = (rfcomm_data *)nrf.tx_buffer_ptr;
  packetRX = (rfcomm_data *)nrf.rx_buffer_ptr;
  
  packetTX->control_flags.is_fragmeneted    = NO;
  packetTX->control_flags.version	    = RFCOMM_VERSION;
  packetTX->control_flags.is_broadcast	    = NO;
  packetTX->data_information.data_type	    = SENSOR_INFO_DATA_TYPE;

  uint8_t magic[] = {0xAA, 0xBB};
  memcpy (&(packetTX->magic_number), magic, 2);
  memcpy (packetTX->sender, local_address, 5);
  memcpy (packetTX->target, packetRX->sender, 5);

  printBuffer ("Sending to ", packetRX->sender, 5);
  net->sendPacket (packetRX->sender);
  net->sendPacket (packetRX->sender);
  net->sendPacket (packetRX->sender);
}

void
network_layer_data_arrived_handler () {
  Serial.println ("network_layer_data_arrived_handler");
}

void hardware_layer_data_arrived_handler () {
  Serial.println ("hardware_layer_data_arrived_handler");
  net->parseRXRawData ();
}

void setup () {
  Serial.begin(9600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  net = new WiseRFComm(&nrf, tx, rx, hardware_layer_data_arrived_handler);
  net->setSender (local_address);
  net->setDataHandler (network_layer_data_arrived_handler);
  net->setBroadcastHandler (network_layer_broadcast_arrived_handler);
  
  printDetails ();

  Serial.println("TLM :: Initialized...");
}

void loop () {
  nrf.pollListener ();
}
