/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <stdint.h>
#include "wise_rfcomm.h"
#include "common.h"

void
send_sensors_data (device_context_t * context, WiseRFComm* network) {
  rfcomm_data*         nrfTXPacket           = (rfcomm_data *)network->tx_ptr;
  rfcomm_sensor_info*  next_sensor_info_slot = (rfcomm_sensor_info *)nrfTXPacket->data_frame.unframeneted.data;
  
  Serial.println ("(TLM)# Sending DATA sensors");
  
  memset (network->tx_ptr, 0, 32);
  nrfTXPacket->data_information.data_size    = 0;
  nrfTXPacket->control_flags.is_fragmeneted  = NO;
  nrfTXPacket->control_flags.version	     = RFCOMM_VERSION;
  nrfTXPacket->control_flags.is_broadcast    = NO;
  nrfTXPacket->data_information.data_type    = SENSOR_INFO_DATA_TYPE;
  
  uint8_t* data_ptr = NULL;
  sensor_t* sensor_info = NULL;
  for (int i = 0; i < context->mapping_size; i++) {
    sensor_info = &(context->mapping_ptr[i]);
    /* Fill the sensor info fields */
    data_ptr = (uint8_t *)next_sensor_info_slot;
    next_sensor_info_slot->sensor_address   = sensor_info->address;
    next_sensor_info_slot->sensor_type      = sensor_info->type;
    next_sensor_info_slot->sebsor_data_len  = 0x1;
    data_ptr += SENSOR_INFO_DATA_SIZE;  
  
    /* Add to the end of the sensor info fields the sesnsor data */
    *data_ptr = (uint8_t)sensor_info->value;
    data_ptr++;
    
    /* Add the sensor data and info size to the packet info */
    nrfTXPacket->data_information.data_size   += (SENSOR_INFO_DATA_SIZE + next_sensor_info_slot->sebsor_data_len);

    /* Set the pointer to the next sensor to be field */
    next_sensor_info_slot = (rfcomm_sensor_info*)data_ptr; 
  }
  
  uint8_t magic[] = {0xAA, 0xBB};
  memcpy (&(nrfTXPacket->magic_number), magic, 2);
  memcpy (nrfTXPacket->sender, context->local_address, 5);
  memcpy (nrfTXPacket->target, context->server_address, 5);
  network->sendPacket (context->server_address);
}

void
send_discovery (device_context_t * context, WiseRFComm* network) {
  Serial.println ("(TLM)# Sending DISCOVERY");
  rfcomm_data*         nrfTXPacket           = (rfcomm_data *)network->tx_ptr;
  rfcomm_device_prot*  prot                  = (rfcomm_device_prot *)nrfTXPacket->data_frame.unframeneted.data;
  
  memset (network->tx_ptr, 0, 32);
  nrfTXPacket->data_information.data_size    = 0;
  nrfTXPacket->control_flags.is_fragmeneted  = NO;
  nrfTXPacket->control_flags.version	     = RFCOMM_VERSION;
  nrfTXPacket->control_flags.is_broadcast    = YES;
  nrfTXPacket->data_information.data_type    = DEVICE_PROT_DATA_TYPE;
  prot->device_cmd                           = DEVICE_PROT_CONNECT_REQ;
  
  uint8_t magic[] = {0xAA, 0xBB};
  memcpy (&(nrfTXPacket->magic_number), magic, 2);
  memcpy (nrfTXPacket->sender, context->local_address, 5);
  memcpy (nrfTXPacket->target, context->server_address, 5);
  network->sendPacket (context->server_address);
}

uint8_t
check_sensors_change (device_context_t * context) {
  uint8_t xor_value = 1;
  for (int i = 0; i < context->mapping_size; i++) {
    xor_value ^= context->mapping_ptr[i].value;
  }
  if (context->xor_sensors_value != xor_value) {
    context->xor_sensors_value = xor_value;
    return YES;
  } else {
    return NO;
  }
}

uint8_t
data_noise_reduse (uint16_t data_prev, uint16_t data_new, uint8_t delta) {
  if (abs (data_prev - data_new) > delta) {
    return data_new;
  } else {
    return data_prev;
  }
}

