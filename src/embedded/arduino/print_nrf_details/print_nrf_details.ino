#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"

#define YES		0x1
#define NO		0x0

//uint8_t local_address[5]     = {0x12, 0x34, 0x56, 0x78, 0x90};
uint8_t local_address[5]     = {0x01, 0x01, 0x01, 0x01, 0x01};

uint8_t demo_address[5]      = {0x01, 0x01, 0x01, 0x01, 0x01};
uint8_t broadcast_address[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t temperature_address   = 0x1;
uint8_t luminance_address     = 0x2;
uint8_t movement_address      = 0x3;

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);
// nrf_module    		nrf     		= nrf_module(&spi);
rfcomm_data*   		packet 			= NULL;
rfcomm_sensor_info*	next_sensor_info_slot 	= NULL;

void hardware_layer_data_arrived_handler () {
  
}

void setup () {
  Serial.begin(9600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  nrf.rx_buffer_ptr = rx;
  nrf.tx_buffer_ptr = tx;
  nrf.setSourceAddress((byte *)local_address);
  nrf.setDestinationAddress((byte *)demo_address);
  nrf.setPayload (32);
  nrf.configure ();
  nrf.setSpeedRate (NRF_250KBPS);
  nrf.dataRecievedHandler = hardware_layer_data_arrived_handler;
  
  packet = (rfcomm_data *)nrf.tx_buffer_ptr;
  Serial.println("TLM :: Initialized...");
  nrf.nrf_print_details();
}

uint8_t is_broadcast  = 0;
uint8_t stage         = 0;
void loop () {
}
