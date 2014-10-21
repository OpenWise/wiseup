#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"

#define YES		0x1
#define NO		0x0

typedef enum {
    DISCOVERY       =  0,
    CONNECTED       =  1
} device_status_t;

uint8_t local_address[5]     = {0x01, 0x02, 0x03, 0x04, 0x05};
uint8_t server_address[5]    = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};
uint8_t broadcast_address[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t temperature_address   = 0x1;
uint8_t luminance_address     = 0x2;
uint8_t movement_address      = 0x3;

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		= spi_interface();
NRF24L01    		nrf     		= NRF24L01(&spi);
rfcomm_data*   		packet 			= NULL;
rfcomm_sensor_info*	next_sensor_info_slot 	= NULL;

uint32_t                sensor_info_timestamp     = 0;
uint32_t                sensor_gateway_timestamp  = 0;
device_status_t         state                     = DISCOVERY;

void
printBuffer (char* name, uint8_t* buff, int len) {
    Serial.print (name);
    for (int i = 0; i < len; i++) {
        Serial.print (buff[i], HEX);
    } Serial.println ();
}

void
hardware_layer_data_arrived_handler () {
  packet = (rfcomm_data *)nrf.rx_buffer_ptr;
  uint8_t data_type = packet->data_information.data_type;
  
  if (data_type == DEVICE_PROT_DATA_TYPE) {
    rfcomm_device_prot* prot = (rfcomm_device_prot*)packet->data_frame.unframeneted.data;
    switch (prot->device_cmd) {
      case DEVICE_PROT_CONNECT_ADDR:
        memcpy (server_address, packet->sender, 5);
        state = CONNECTED;
      break;
    }
  }
}

void
setup () {
  Serial.begin(9600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  nrf.rx_buffer_ptr = rx;
  nrf.tx_buffer_ptr = tx;
  nrf.setSourceAddress((byte *)local_address);
  nrf.setDestinationAddress((byte *)server_address);
  nrf.setPayload (32);
  nrf.configure ();
  nrf.setSpeedRate (NRF_250KBPS);
  nrf.setChannel (99);
  nrf.dataRecievedHandler = hardware_layer_data_arrived_handler;
  
  sensor_info_timestamp    = millis ();
  sensor_gateway_timestamp = millis ();
  packet = (rfcomm_data *)nrf.tx_buffer_ptr;
  Serial.println("TLM :: Initialized...");
}

void
loop () {
  nrf.pollListener ();
 
  /* SENSOR INFO */
  if (abs (millis () - sensor_info_timestamp) > 3000) {
    memset (tx, 0, 32);
    packet = (rfcomm_data *)nrf.tx_buffer_ptr;
    next_sensor_info_slot = (rfcomm_sensor_info *)packet->data_frame.unframeneted.data;
    packet->data_information.data_size = 0;
    
    add_analog_temperature_info (0, temperature_address);
    add_analog_luminance_info (2, luminance_address);
  
    packet->control_flags.is_fragmeneted  = NO;
    packet->control_flags.version	  = RFCOMM_VERSION;
    packet->control_flags.is_broadcast	  = NO;
    packet->data_information.data_type	  = SENSOR_INFO_DATA_TYPE;

    uint8_t magic[] = {0xAA, 0xBB};
    memcpy (&(packet->magic_number), magic, 2);
    memcpy (packet->sender, local_address, 5);

    Serial.print ("Sending to ");
    printBuffer ("", server_address, 5);
    nrf.setDestinationAddress((byte *)server_address);
    nrf.send ();
    
    sensor_info_timestamp = millis ();
  }
  
  /* CHECK GATEWAY */
  if (abs (millis () - sensor_gateway_timestamp) > 30000) {
    sensor_gateway_timestamp = millis ();
  }
}

uint8_t 
add_analog_temperature_info (uint8_t pin, uint8_t addr) {
  int     tempSum     = 0;
  float   temperature = 0;
  
  /* Read data from sensor 10 times */
  for (int i = 0; i < 10; i++) {
    tempSum += analogRead (pin);
  }
  temperature = (tempSum * 0.48828125) / 10;
  
  /* Fill the sensor info fields */
  uint8_t* data_ptr = (uint8_t *)next_sensor_info_slot;
  next_sensor_info_slot->sensor_address   = addr;
  next_sensor_info_slot->sensor_type      = TEMPERATURE_SENSOR_TYPE;
  next_sensor_info_slot->sebsor_data_len  = 0x1;
  data_ptr += SENSOR_INFO_DATA_SIZE;
  
  /* Add to the end of the sensor info fields the sesnsor data */
  *data_ptr = temperature;
  data_ptr++;
  
  /* Add the sensor data and info size to the packet info */
  packet->data_information.data_size   += (SENSOR_INFO_DATA_SIZE + next_sensor_info_slot->sebsor_data_len);
  
  /* Set the pointer to the next sensor to be field */
  next_sensor_info_slot = (rfcomm_sensor_info*)data_ptr;
  
  Serial.print ("LM35 - ");
  Serial.println ((uint8_t) temperature);
  
  return 0;
}

uint8_t 
add_analog_luminance_info (uint8_t pin, uint8_t addr) {
  int   lumSum     = 0;
  float luminance  = 0;
  
  /* Fill the sensor info fields */
  for (int i = 0; i < 10; i++) {
    lumSum += analogRead (pin);
  }
  luminance = (lumSum * 0.48828125) / 10;
  luminance = (luminance / 512) * 100;
  
  /* Fill the sensor info fields */
  uint8_t* data_ptr = (uint8_t *)next_sensor_info_slot;
  next_sensor_info_slot->sensor_address   = addr;
  next_sensor_info_slot->sensor_type      = LUMINANCE_SENSOR_TYPE;
  next_sensor_info_slot->sebsor_data_len  = 0x1;
  data_ptr += SENSOR_INFO_DATA_SIZE;
  
  /* Add to the end of the sensor info fields the sesnsor data */
  *data_ptr = (uint8_t)luminance;
  data_ptr++;
  
  /* Add the sensor data and info size to the packet info */
  packet->data_information.data_size   += (SENSOR_INFO_DATA_SIZE + next_sensor_info_slot->sebsor_data_len);
  
  /* Set the pointer to the next sensor to be field */
  next_sensor_info_slot = (rfcomm_sensor_info*)data_ptr;
  
  Serial.print ("LDR - ");
  Serial.println ((uint8_t) luminance);
  
  return 0;
}

