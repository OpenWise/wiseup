#include <OneWire.h>
#include <DallasTemperature.h>
#include <nRFModule.h>
#include <SPInterface.h>
#include "wise_rfcomm.h"
#include "sensors/sensors.h"
#include "sensors/common.h"
#include "sensors/nrf24l01_receiver_handler.h"
#include "sensors/device_methods.h"

#define DISCOVERY_MODE_INTERVAL                             120000
#define DISCOVERY_MODE_TIMEOUT                              5000
#define CONNECTED_MODE_READ_SENSORS_INTERVAL                1000
#define CONNECTED_MODE_READ_SENSORS_AS_KEEPALIVE_INTERVAL   30000

#define ANALOG_LDR_ADDR             1

#define DIGITAL_TEMPERATURE_PIN     2
#define ANALOG_LDR_PIN              2
#define DIGITAL_PIR_PIN             4
#define DIGITAL_RELAY_PIN           5

uint8_t rx[32];
uint8_t tx[32];

spi_interface 		    spi     		  = spi_interface();
NRF24L01    		    nrf     		  = NRF24L01(&spi);
WiseRFComm*             net               = NULL;

rfcomm_data*   		nrfRXPacket 		  = NULL;
rfcomm_data*   		nrfTXPacket 		  = NULL;
rfcomm_device_prot*     prot              = NULL;
rfcomm_sensor_info*	next_sensor_info_slot = NULL;

#define MAPPING_SIZE    1
sensor_t mapping[] = {
  { ANALOG_LDR_ADDR, ANALOG_LDR_PIN, LUMINANCE_SENSOR_TYPE, 0 , 30 }
};

device_context_t device_context = { mapping, MAPPING_SIZE, DISCOVERY, 0, 0,
                                    {0xFA, 0xFA, 0xFA, 0xFA, 0xFA}, 
                                    {0x06, 0x04, 0x03, 0x02, 0x01},
                                    {0xFA, 0xFA, 0xFA, 0xFA, 0xFA},
                                    millis(), millis(), millis(), millis() };

void setup () {
    Serial.begin(9600);
    nrf.init (10, 9); // default csn = 7, ce = 8 (10, 9 on imall board)
    net = new WiseRFComm(&nrf, tx, rx, NULL);
    net->setSender (device_context.local_address);

    nrfRXPacket = (rfcomm_data *)net->rx_ptr;
    nrfTXPacket = (rfcomm_data *)net->tx_ptr;

    Serial.println("(TLM)# Initialized...");
}

void loop () {
    // Read sensors data each second
    if (abs (millis () - device_context.connected_read_sensors_interval) > CONNECTED_MODE_READ_SENSORS_INTERVAL) {
        device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].value = 
            data_noise_reduse( device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].value,
                (uint16_t) read_analog_luminance_info (device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].pin), 2);

        // Check for changes
        device_context.is_sync = check_sensors_change (&device_context);
        device_context.connected_read_sensors_interval = millis ();
    }

    if (abs (millis () - device_context.connected_read_sensors_as_keepalive_interval) > CONNECTED_MODE_READ_SENSORS_AS_KEEPALIVE_INTERVAL) {
        send_sensors_data_no_auth (&device_context, net);
        device_context.connected_read_sensors_as_keepalive_interval = millis ();
    }

    if (device_context.is_sync == YES) {
        // Send packet with sensors data
        send_sensors_data_no_auth (&device_context, net);
        device_context.is_sync = NO;

        Serial.print ("(TLM)# Sensors : ");
        Serial.print ((uint8_t)device_context.mapping_ptr[ANALOG_LDR_ADDR - 1].value);
        Serial.println ();
    }
}

