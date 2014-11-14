#include <VirtualWire.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include "wise_rfcomm.h"

#define ONE_WIRE_BUS   2
#define TX_PIN         1
#define SPEED_RATE     1000

uint8_t local_address[6] = {0x02, 0x02, 0x02, 0x02, 0x02, 0x01};

typedef struct {
 rfcomm_individual_sensor_info  info;
 uint8_t                        data;
} sensor_data; 

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

sensor_data  sensor;

void setup() {
  pinMode (TX_PIN,OUTPUT);
  vw_set_ptt_inverted (true);
  vw_set_tx_pin (TX_PIN);
  vw_setup(SPEED_RATE); // speed of data transfer Kbps
  sensors.begin();
}

void loop() {
  sensors.requestTemperatures();
  
  sensor.data = (uint8_t)sensors.getTempCByIndex(0);
  sensor.info.sensor_type = TEMPERATURE_SENSOR_TYPE;
  sensor.info.sensor_data_len = sizeof(sensor_data);
  memcpy (sensor.info.sensor_address, local_address, 6);
  
  vw_send((uint8_t *)&sensor, sizeof(sensor_data));
  vw_wait_tx(); // Wait until the whole message is gone

  delay(5000);
}
