/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "wise_client_handler.h"
#include "commonMethods.hpp"
#include "wiseDBMng.h"

using namespace std;

extern uint8_t local_address[5];

WiseClientHandler::WiseClientHandler (comm::WiseRFComm * net) {
    m_net = net;
}

WiseClientHandler::~WiseClientHandler () {
}

wise_status_t
WiseClientHandler::registrationCheck (rfcomm_data* wisePacket) {
    WiseClient* device = findClient (wisePacket->sender);
	
	CommonMethods::printBuffer("(WiseClientHandler) [registrationCheck] ", wisePacket->sender, 5);
    
    if (wisePacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
        rfcomm_device_prot* prot = (rfcomm_device_prot*)wisePacket->data_frame.unframeneted.data;
        
        if (prot->device_cmd == DEVICE_PROT_CONNECT_REQ) {
            if (device != NULL) {
                /* Update timestamp */
                device->timestamp = (uint64_t)time(NULL);
                
                if (device->status == DISCOVERY) {
                    return DISCOVERY;
                } else if (device->status == CONNECTED) {
					WiseDBMng::apiSetSensorAvailability (wisePacket, true); // Set HUB as available
                    return CONNECTED;
                }
            } else {
                /* 
                 * New device, let us add it to the list of devices and
                 * send our address back to the device. When the device will recieve 
                 * our address it will stop broadcasting and know this gateway.
                 */
                
                WiseClient client   = WiseClient(wisePacket->sender);
                client.timestamp    = (uint64_t)time(NULL);
                client.status       = DISCOVERY;
                m_clients.push_back(client);
				
				printf ("(WiseClientHandler) [registrationCheck] Adding new device \n");
			
                return DISCOVERY;
            }
        }
    } else {
        if (device != NULL) {
            device->timestamp = (uint64_t)time(NULL);
			WiseDBMng::apiSetSensorAvailability (wisePacket, true); // Set HUB as available
            return CONNECTED;
        }
    }
    
    return UNKNOWN;
}

void
WiseClientHandler::removeUnusedDeveices () {
    for (std::vector<WiseClient>::iterator item = m_clients.begin(); item != m_clients.end(); ++item) {
        if ( (uint64_t)time(NULL) - item->timestamp > 60) {
            printf ("(WiseClientHandler) [removeUnusedDeveices] ");
            item->printAddress();
            
			rfcomm_data packet;
			memcpy(packet.sender, item->address, 5);
			WiseDBMng::apiSetSensorAvailability (&packet, false); // Set HUB as NOT available
			
			m_clients.erase (item);
			
			return;
        }
    }
}

WiseClient* 
WiseClientHandler::findClient (uint8_t * address) {
    WiseClient client = WiseClient(address);
    
    if (m_clients.empty()) {
    } else {
        /* Lets try to find this device in the list */
        for (std::vector<WiseClient>::iterator item = m_clients.begin(); item != m_clients.end(); ++item) {
            if (*item == client) {
                return &(*item);
            }
        }
    }
    
    return NULL;
}

void
WiseClientHandler::sendRegistration () {
    nrf24l01_msg_t msg;
    rfcomm_data *wisePacketRX = (rfcomm_data *)m_net->ptrRX;
    rfcomm_data *wisePacketTX = (rfcomm_data *)msg.packet;

    /* Create package */
    wisePacketTX->data_information.data_type      = DEVICE_PROT_DATA_TYPE;
    wisePacketTX->data_information.data_size      = DEVICE_PROT_CONN_DATA_SIZE;
    wisePacketTX->control_flags.is_fragmeneted    = 0;
    wisePacketTX->control_flags.version           = 1;
    wisePacketTX->control_flags.is_broadcast      = 0;
    wisePacketTX->control_flags.is_ack            = 0;
    wisePacketTX->magic_number[0]                 = 0xAA;
    wisePacketTX->magic_number[1]                 = 0xBB;
    memcpy (wisePacketTX->sender, local_address, 5);
    memcpy (wisePacketTX->target, wisePacketRX->sender, 5);
    rfcomm_device_prot* deviceProt              = (rfcomm_device_prot *)wisePacketTX->data_frame.unframeneted.data;
    deviceProt->device_cmd                      = DEVICE_PROT_CONNECT_ADDR;
    memcpy (deviceProt->device_data, local_address, 5);
	
	memcpy(msg.packet, wisePacketTX, 32);
	msg.isGeneratePacketID = true;

    /* Send to OUTGOING queue */
    WiseIPC *ipcPacketsOut = new WiseIPC ("/tmp/wiseup/nrf_outgoing_queue");
    if (ipcPacketsOut->setClient () == SUCCESS) {
        ipcPacketsOut->setBuffer((unsigned char *)&msg);
        if (ipcPacketsOut->sendMsg(sizeof (nrf24l01_msg_t)) == true) {
			CommonMethods::printBuffer ("(WiseClientHandler) [sendRegistration] Registration for ", wisePacketTX->target, 5);
		}
        else { }
    }

    delete ipcPacketsOut;
}

WiseCommandHandler::WiseCommandHandler (comm::WiseRFComm * net) {
    m_net = net;
}

WiseCommandHandler::~WiseCommandHandler () {
}

void
WiseCommandHandler::commandHandler (rfcomm_data* wisePacket) {    
    if (wisePacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
        rfcomm_device_prot* prot = (rfcomm_device_prot*)wisePacket->data_frame.unframeneted.data;
        
        switch (prot->device_cmd) {
            case DEVICE_PROT_CONNECT_CHK:
                printf ("(wise-nrfd) [WiseCommandHandler::commandHandler] DEVICE_PROT_CONNECT_CHK");
                rfcomm_data *wisePacketRX = (rfcomm_data *)m_net->ptrRX;
                rfcomm_data *wisePacketTX = (rfcomm_data *)m_net->ptrTX;
                wisePacketTX->data_information.data_type      = DEVICE_PROT_DATA_TYPE;
                wisePacketTX->data_information.data_size      = DEVICE_PROT_CONN_DATA_SIZE;
                wisePacketTX->control_flags.is_fragmeneted    = 0;
                wisePacketTX->control_flags.version           = 1;
                wisePacketTX->control_flags.is_broadcast      = 0;
                wisePacketTX->magic_number[0]                 = 0xAA;
                wisePacketTX->magic_number[1]                 = 0xBB;
                memcpy (wisePacketTX->sender, local_address, 5);
                memcpy (wisePacketTX->target, wisePacketRX->sender, 5);
                rfcomm_device_prot* deviceProt              = (rfcomm_device_prot *)wisePacketTX->data_frame.unframeneted.data;
                deviceProt->device_cmd                      = DEVICE_PROT_CONNECT_CHK;
                
                // m_net->sendPacket (wisePacketRX->sender);
            break;
        }
    }
}
