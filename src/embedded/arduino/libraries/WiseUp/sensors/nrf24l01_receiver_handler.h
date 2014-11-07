/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <stdint.h>
#include "wise_rfcomm.h"
#include "common.h"

void
nrf24l01_receiver_handler (rfcomm_data* nrfRXPacket, device_context_t * device_info) {
  Serial.println ("(TLM)# Got WiseUp packet");
  if (nrfRXPacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
    rfcomm_device_prot* prot = (rfcomm_device_prot *)nrfRXPacket->data_frame.unframeneted.data;
    if (prot->device_cmd == DEVICE_PROT_CONNECT_ADDR) {
      memcpy (device_info->server_address, prot->device_data, 5);
      Serial.println ("(TLM)# Change MODE (CONNECTED)");
      device_info->state               = CONNECTED;
      device_info->discovery_interval  = millis ();
    }
    if (prot->device_cmd == DEVICE_PROT_CONNECT_CHK) {
      device_info->discovery_interval = millis ();
    }
  }
  
  if (nrfRXPacket->data_information.data_type == SENSOR_CMD_DATA_TYPE) {
    rfcomm_sensor_command* sensorCmd = 
              (rfcomm_sensor_command *)nrfRXPacket->data_frame.unframeneted.data;
    switch (sensorCmd->command_type) {
      case SENSOR_CMD_RELAY:
        for (int i = 0; i < device_info->mapping_size; i++) {
          if (device_info->mapping_ptr[i].address == sensorCmd->sensor_address) {
            write_digital_relay_info (device_info->mapping_ptr[i].pin, sensorCmd->command_data[0]);
            // Send response back
            device_info->is_sync = YES;
            return;
          }
        }
        break;
      case SENSOR_CMD_RELAY_RGB:
        break;
    }
  }
}

