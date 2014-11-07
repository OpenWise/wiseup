#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"

#define YES		0x1
#define NO		0x0

uint8_t local_address[5]     = {0x01, 0x01, 0x01, 0x01, 0x02};
uint8_t demo_address[5]      = {0x01, 0x01, 0x01, 0x01, 0x01};
uint8_t broadcast_address[5] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF};

uint8_t temperature_address   = 0x1;
uint8_t luminance_address     = 0x2;
uint8_t movement_address      = 0x3;

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		= spi_interface();
nrf_module    		nrf     		= nrf_module(&spi);
rfcomm_data*   		packet 			= NULL;
rfcomm_sensor_info*	next_sensor_info_slot 	= NULL;

void hardware_layer_data_arrived_handler () {
  
}

void setup () {
  Serial.begin(9600);
  nrf.nrf_init_module(7, 8); // default csn = 7, ce = 8 (10, 9 on imall board)
  nrf.rx_buffer_ptr = rx;
  nrf.tx_buffer_ptr = tx;
  nrf.nrf_set_raddr((byte *)local_address);
  nrf.nrf_set_taddr((byte *)demo_address);
  nrf.nrf_set_payload(32);
  nrf.nrf_config_module();
  nrf.nrf_data_recieved_handler	= hardware_layer_data_arrived_handler;
  
  packet = (rfcomm_data *)nrf.tx_buffer_ptr;
  Serial.println("TLM :: Initialized...");
}

uint8_t is_broadcast  = 0;
uint8_t stage         = 0;
void loop () {
  nrf.nrf_listen_for_channel();
 
  memset (tx, 0, 32);
  next_sensor_info_slot = (rfcomm_sensor_info *)packet->data_frame.unframeneted.data;
  packet->data_information.data_size = 0;
  
  add_temperature_info (14, temperature_address);
  add_luminance_info   (16, luminance_address);

  if (!is_broadcast) {
    packet->control_flags.is_fragmeneted  = NO;
    packet->control_flags.version	  = RFCOMM_VERSION;
    packet->control_flags.is_broadcast	  = NO;
    packet->data_information.data_type	  = SENSOR_INFO_DATA_TYPE;

    uint8_t magic[] = {0xAA, 0xBB};
    memcpy (&(packet->magic_number), magic, 2);
    memcpy (packet->sender, local_address, 5);

    Serial.print ("Sending ..... ");
    nrf.nrf_set_taddr((byte *)demo_address);
    nrf.nrf_send();
    
    Serial.println ("Data sent\n");
    delay(3000);
  }
}

uint8_t 
add_temperature_info (uint8_t pin, uint8_t addr) {
  int     tempSum     = 0;
  int     measurement = 0;
  float   temperature = 0;
  
  /* Read data from sensor 10 times */
  if (pin > 13 && pin < 20) {
    for (int i = 0; i < 10; i++) {
      measurement = analogRead (pin);
      tempSum += measurement;
      Serial.print ("Measurement ");
      Serial.print (i);
      Serial.print (" = ");
      Serial.println (measurement);
    }
    temperature = (tempSum * 0.48828125) / 10;
    Serial.print ("LM35 - ");
  } else if (pin > 1 && pin < 14) { }
  
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
  
  Serial.println ((uint8_t) temperature);
}

uint8_t 
add_luminance_info (uint8_t pin, uint8_t addr) {
  int   lumSum     = 0;
  int     measurement = 0;
  float luminance  = 0;
  
  /* Fill the sensor info fields */
  if (pin > 13 && pin < 20) {
    for (int i = 0; i < 10; i++) {
      measurement = analogRead (pin);
      lumSum += measurement;
      Serial.print ("Measurement ");
      Serial.print (i);
      Serial.print (" = ");
      Serial.println (measurement);
    }
    luminance = (lumSum * 0.48828125) / 10;
    luminance = (luminance / 512) * 100;
  }
  
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

