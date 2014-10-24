/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <stdint.h>
#include "wise_rfcomm.h"
#include "common.h"

void
nrf24l01_receiver_handler (device_context_t * context, WiseRFComm* network) {
  rfcomm_data*  nrfTXPacket  = (rfcomm_data *)network->tx_ptr;
  rfcomm_data*  nrfRXPacket  = (rfcomm_data *)network->rx_ptr;
  
  Serial.println ("(TLM)# Got WiseUp packet");
  if (nrfRXPacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
    rfcomm_device_prot* prot = (rfcomm_device_prot *)nrfRXPacket->data_frame.unframeneted.data;
    if (prot->device_cmd == DEVICE_PROT_CONNECT_ADDR) {
      memcpy (context->server_address, prot->device_data, 5);
      Serial.println ("(TLM)# Change MODE (CONNECTED)");
      context->state               = CONNECTED;
      context->discovery_interval  = millis ();
    }
    if (prot->device_cmd == DEVICE_PROT_CONNECT_CHK) {
      context->discovery_interval = millis ();
    }
  }
  
  if (nrfRXPacket->data_information.data_type == SENSOR_CMD_DATA_TYPE) {
    rfcomm_sensor_command* sensorCmd = 
              (rfcomm_sensor_command *)nrfRXPacket->data_frame.unframeneted.data;
    switch (sensorCmd->command_type) {
      case SENSOR_CMD_RELAY:
        for (int i = 0; i < context->mapping_size; i++) {
          if (context->mapping_ptr[i].address == sensorCmd->sensor_address) {
            write_digital_relay_info (context->mapping_ptr[i].pin, sensorCmd->command_data[0]);
            Serial.println ((uint16_t)nrfRXPacket->packet_id);
            // Send response back
            if (nrfRXPacket->control_flags.is_ack) {
              memcpy (nrfTXPacket, nrfRXPacket, 32);
              memcpy (nrfTXPacket->target, context->server_address, 5);
              memcpy (nrfTXPacket->sender, context->local_address, 5);
              network->sendPacket (context->server_address);
            }
            
            delay (100);
            context->is_sync = YES;
            return;
          }
        }
        break;
      case SENSOR_CMD_RELAY_RGB:
        break;
    }
  }
}

