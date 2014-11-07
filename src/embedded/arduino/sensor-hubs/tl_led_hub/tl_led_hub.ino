#include <OneWire.h>
#include <DallasTemperature.h>
#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"
#include "sensors/sensors.h"
#include "sensors/common.h"
#include "sensors/nrf24l01_receiver_handler.h"
#include "sensors/device_methods.h"

#define DISCOVERY_MODE_INTERVAL		                  120000
#define DISCOVERY_MODE_TIMEOUT                            5000
#define CONNECTED_MODE_READ_SENSORS_INTERVAL              1000
#define CONNECTED_MODE_READ_SENSORS_AS_KEEPALIVE_INTERVAL 30000

#define DIGITAL_TEMPERATURE_ADDR    1
#define ANALOG_LDR_ADDR             2
#define DIGITAL_RELAY_ADDR          3

#define DIGITAL_TEMPERATURE_PIN     2
#define ANALOG_LDR_PIN              2
#define DIGITAL_RELAY_PIN           5

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		spi     		  = spi_interface();
NRF24L01    		nrf     		  = NRF24L01(&spi);
WiseRFComm*             net                       = NULL;

rfcomm_data*   		nrfRXPacket 		  = NULL;
rfcomm_data*   		nrfTXPacket 		  = NULL;
rfcomm_device_prot*     prot                      = NULL;
rfcomm_sensor_info*	next_sensor_info_slot 	  = NULL;

#define MAPPING_SIZE    3
sensor_t mapping[] = {
  { DIGITAL_TEMPERATURE_ADDR,  DIGITAL_TEMPERATURE_PIN,  TEMPERATURE_SENSOR_TYPE, 0 },
  { ANALOG_LDR_ADDR,           ANALOG_LDR_PIN,           LUMINANCE_SENSOR_TYPE,   0 },
  { DIGITAL_RELAY_ADDR,        DIGITAL_RELAY_PIN,        RELAY_SENSOR_TYPE,       0 }
};

device_context_t device_context = { mapping, MAPPING_SIZE, DISCOVERY, 0, 0,
                                    {0xFA, 0xFA, 0xFA, 0xFA, 0xFA}, 
                                    {0x05, 0x04, 0x03, 0x02, 0x02},
                                    {0xFA, 0xFA, 0xFA, 0xFA, 0xFA},
                                    millis(), millis(), millis(), millis() };

OneWire oneWire(DIGITAL_TEMPERATURE_PIN);
DallasTemperature sensors(&oneWire);

void
network_layer_broadcast_arrived_handler () {
  Serial.println ("(TLM)# Broadcast data [NULL]");
}

void
network_layer_data_arrived_handler () {
  nrf24l01_receiver_handler (nrfRXPacket, &device_context);
}

void
hardware_layer_data_arrived_handler () {
  net->parseRXRawData ();
}

void setup () {
  Serial.begin(57600);
  nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
  net = new WiseRFComm(&nrf, tx, rx, hardware_layer_data_arrived_handler);
  net->setSender (device_context.local_address);
  net->setDataHandler (network_layer_data_arrived_handler);
  net->setBroadcastHandler (network_layer_broadcast_arrived_handler);
  
  nrfRXPacket = (rfcomm_data *)net->rx_ptr;
  nrfTXPacket = (rfcomm_data *)net->tx_ptr;
  
  pinMode (device_context.mapping_ptr[DIGITAL_RELAY_ADDR - 1].pin, OUTPUT);
  
  sensors.begin();
  
  Serial.println("(TLM)# Initialized...");
}

void loop () {
  net->listenForIncoming ();
  
  // Change to DISCOVERY mode
  if (abs (millis () - device_context.discovery_interval) > DISCOVERY_MODE_INTERVAL) {
    Serial.println ("(TLM)# Change MODE (DISCOVERY)");
    device_context.state = DISCOVERY;
    device_context.discovery_interval = millis ();
  }
  
  // STATE MACHINE
  switch (device_context.state) {
    case DISCOVERY:
      if (abs (millis () - device_context.discovery_timeout) > DISCOVERY_MODE_TIMEOUT) {
        Serial.println ("(TLM)# DISCOVERY");
        device_context.state = DISCOVERY;
        memcpy (device_context.server_address, device_context.broadcast_address, 5);
        // Send DISCOVERY packet
        send_discovery (&device_context, net);
        device_context.discovery_timeout = millis ();
      }
    break;
    case CONNECTED:
      // Read sensors data each second
      if (abs (millis () - device_context.connected_read_sensors_interval) > CONNECTED_MODE_READ_SENSORS_INTERVAL) {
        Serial.println ("(TLM)# CONNECTED");
        device_context.mapping_ptr[DIGITAL_TEMPERATURE_ADDR - 1].value = 
                  data_noise_reduse( device_context.mapping_ptr[DIGITAL_TEMPERATURE_ADDR - 1].value, 
                                    (uint16_t) read_digital_temperature_info (&sensors), 2);
        device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].value = 
                  data_noise_reduse( device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].value,
                                    (uint16_t) read_analog_luminance_info (device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].pin), 2);
        device_context.mapping_ptr[DIGITAL_RELAY_ADDR - 1].value = 
                  (uint16_t) read_digital_relay_info (device_context.mapping_ptr[DIGITAL_RELAY_ADDR - 1].pin);

        // Check for changes
        device_context.is_sync = check_sensors_change (&device_context);
        device_context.connected_read_sensors_interval = millis ();
      }
      
      if (abs (millis () - device_context.connected_read_sensors_as_keepalive_interval) > CONNECTED_MODE_READ_SENSORS_AS_KEEPALIVE_INTERVAL) {
        send_sensors_data (&device_context, net);
        device_context.connected_read_sensors_as_keepalive_interval = millis ();
      }
      
      if (device_context.is_sync == YES) {
        // Send packet with sensors data
        send_sensors_data (&device_context, net);
        device_context.is_sync = NO;
        
        Serial.print ("(TLM)# Sensors : ");
        Serial.print ((uint8_t)device_context.mapping_ptr[DIGITAL_TEMPERATURE_ADDR - 1].value);
        Serial.print (" ");
        Serial.print ((uint8_t)device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].value);
        Serial.print (" ");
        Serial.println ((uint8_t)device_context.mapping_ptr[DIGITAL_RELAY_ADDR - 1].value);
      }
    break;
  }
}

