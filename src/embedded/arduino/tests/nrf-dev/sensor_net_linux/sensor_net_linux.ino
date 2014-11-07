#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"

#define YES		0x1
#define NO		0x0

typedef enum {
    DISCOVERY = 0,
    CONNECTED = 1
} device_status_t;

uint8_t local_address[5]     = {0x05, 0x04, 0x03, 0x02, 0x01};
uint8_t server_address[5]    = {0xFA, 0xFA, 0xFA, 0xFA, 0xFA};
uint8_t broadcat_address[5]  = {0xFA, 0xFA, 0xFA, 0xFA, 0xFA};

uint8_t temperature_address   = 0x1;
uint8_t luminance_address     = 0x2;
uint8_t movement_address      = 0x3;

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		  = spi_interface();
NRF24L01    		nrf     		  = NRF24L01(&spi);
WiseRFComm*             net                       = NULL;

rfcomm_data*   		nrfRXPacket 		  = NULL;
rfcomm_data*   		nrfTXPacket 		  = NULL;
rfcomm_device_prot*     prot                      = NULL;
rfcomm_sensor_info*	next_sensor_info_slot 	  = NULL;

uint32_t                sensor_info_timestamp     = 0;
uint32_t                sensor_gateway_timestamp  = 0;
device_status_t         state                     = DISCOVERY;
uint8_t                 timeout                   = 0;

uint8_t                 pirValue                  = 0;
uint8_t                 pirState                  = 0;
uint32_t                pirLastTimestamp          = 0;

void
printBuffer (char* name, uint8_t* buff, int len) {
    Serial.print (name);
    for (int i = 0; i < len; i++) {
        Serial.print (buff[i], HEX);
    } Serial.println ();
}

void
network_layer_broadcast_arrived_handler () {
  Serial.println ("(TLM)# Broadcast data [NULL]");
}

void
network_layer_data_arrived_handler () {
  Serial.println ("(TLM)# Got WiseUp packet");
  if (nrfRXPacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
    prot = (rfcomm_device_prot *)nrfRXPacket->data_frame.unframeneted.data;
    if (prot->device_cmd == DEVICE_PROT_CONNECT_ADDR) {
      memcpy (server_address, prot->device_data, 5);
      state   = CONNECTED;
      timeout = 0;
    }
    if (prot->device_cmd == DEVICE_PROT_CONNECT_CHK) {
      timeout = 0;
    }
  }
}

void
hardware_layer_data_arrived_handler () {
  Serial.println ("(TLM)# nRF24l01 layer");
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
  sensor_gateway_timestamp = millis ();
  pirLastTimestamp         = millis ();
  nrfRXPacket = (rfcomm_data *)nrf.rx_buffer_ptr;
  nrfTXPacket = (rfcomm_data *)nrf.tx_buffer_ptr;
  
  Serial.println("(TLM)# Initialized...");
}

void
loop () {
  net->listenForIncoming ();
  
  pirValue = digitalRead (5);
  if (pirState != pirValue) {
    pirState = pirValue;
    sendMovement ();
  }
  
  /* CHECK GATEWAY */
  if (abs (millis () - sensor_gateway_timestamp) > 10000) {
    if (timeout > 6) {
      memcpy (server_address, broadcat_address, 5);
      state = DISCOVERY;
    }
    
    timeout++;
    sensor_gateway_timestamp = millis ();
  }
  
  if (abs (millis () - sensor_info_timestamp) > 5000) {
    memset (tx, 0, 32);
    next_sensor_info_slot = (rfcomm_sensor_info *)nrfTXPacket->data_frame.unframeneted.data;
    nrfTXPacket->data_information.data_size = 0;
    
    nrfTXPacket->control_flags.is_fragmeneted  = NO;
    nrfTXPacket->control_flags.version	  = RFCOMM_VERSION;
    
    if (state == DISCOVERY) {
      prot = (rfcomm_device_prot *)nrfTXPacket->data_frame.unframeneted.data;
      nrfTXPacket->control_flags.is_broadcast	  = YES;
      nrfTXPacket->data_information.data_type	  = DEVICE_PROT_DATA_TYPE;
      prot->device_cmd                            = DEVICE_PROT_CONNECT_REQ;
    } else if (state == CONNECTED) {
      nrfTXPacket->control_flags.is_broadcast	  = NO;
      nrfTXPacket->data_information.data_type	  = SENSOR_INFO_DATA_TYPE;
    
      add_analog_temperature_info (0, temperature_address);
      add_analog_luminance_info (2, luminance_address);
    }
    
    uint8_t magic[] = {0xAA, 0xBB};
    memcpy (&(nrfTXPacket->magic_number), magic, 2);
    memcpy (nrfTXPacket->sender, local_address, 5);
    memcpy (nrfTXPacket->target, server_address, 5);

    printBuffer ("(TLM)# Sending to ", server_address, 5);
    net->sendPacket (server_address);
    
    sensor_info_timestamp = millis ();
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
  nrfTXPacket->data_information.data_size   += (SENSOR_INFO_DATA_SIZE + next_sensor_info_slot->sebsor_data_len);

  /* Set the pointer to the next sensor to be field */
  next_sensor_info_slot = (rfcomm_sensor_info*)data_ptr;

  Serial.print ("(TLM)# Temperature is ");
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
  nrfTXPacket->data_information.data_size   += (SENSOR_INFO_DATA_SIZE + next_sensor_info_slot->sebsor_data_len);

  /* Set the pointer to the next sensor to be field */
  next_sensor_info_slot = (rfcomm_sensor_info*)data_ptr;  

  Serial.print ("(TLM)# Luminance is ");
  Serial.println ((uint8_t) luminance); 

  return 0;
}

void
sendMovement () {
  if (abs(millis() - pirLastTimestamp) > 2000) {
    Serial.println("sendMovement");
    
    memset (tx, 0, 32);
    next_sensor_info_slot = (rfcomm_sensor_info *)nrfTXPacket->data_frame.unframeneted.data;
    nrfTXPacket->data_information.data_size       = 0;
    nrfTXPacket->control_flags.is_fragmeneted     = NO;
    nrfTXPacket->control_flags.version	          = RFCOMM_VERSION;
    nrfTXPacket->control_flags.is_broadcast	  = NO;
    nrfTXPacket->data_information.data_type	  = SENSOR_INFO_DATA_TYPE;
    
    uint8_t magic[] = {0xAA, 0xBB};
    memcpy (&(nrfTXPacket->magic_number), magic, 2);
    memcpy (nrfTXPacket->sender, local_address, 5);
    memcpy (nrfTXPacket->target, server_address, 5);
    
    next_sensor_info_slot = (rfcomm_sensor_info *)nrfTXPacket->data_frame.unframeneted.data;
    nrfTXPacket->data_information.data_size = 0;
    
    /* Fill the sensor info fields */
    uint8_t* data_ptr = (uint8_t *)next_sensor_info_slot;
    next_sensor_info_slot->sensor_address   = movement_address;
    next_sensor_info_slot->sensor_type      = PIR_SENSOR_TYPE;
    next_sensor_info_slot->sebsor_data_len  = 0x1;
    data_ptr += SENSOR_INFO_DATA_SIZE;  
  
    /* Add to the end of the sensor info fields the sesnsor data */
    *data_ptr = (uint8_t)pirValue;
    data_ptr++; 
  
    /* Add the sensor data and info size to the packet info */
    nrfTXPacket->data_information.data_size   += (SENSOR_INFO_DATA_SIZE + next_sensor_info_slot->sebsor_data_len);

    printBuffer ("(TLM)# Sending to ", server_address, 5);
    net->sendPacket (server_address);
    
    pirLastTimestamp = millis ();
  }
}

