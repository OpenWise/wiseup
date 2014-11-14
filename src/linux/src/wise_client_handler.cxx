/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>

#include "wise_client_handler.h"
#include "commonMethods.hpp"

using namespace std;

extern uint8_t local_address[5];

WiseClientHandler::WiseClientHandler () {
	struct DBCredentials cred;
	
    cred.hostName   = "localhost";
    cred.userId     = "root";
    cred.password   = "sql123";
    cred.database   = "wiseup";
	
	m_dbconn = new MySQL();
	m_dbconn->setupConnection(&cred);
	
	pthread_cond_init  (&lock.cond, NULL);
	pthread_mutex_init (&lock.mutex, NULL);
}

WiseClientHandler::~WiseClientHandler () {
	pthread_mutex_destroy (&lock.mutex);
    pthread_cond_destroy  (&lock.cond);
	
	delete m_dbconn;
}

void
WiseClientHandler::printClentInfo () {
	pthread_mutex_lock (&lock.mutex);
	if (m_clients.empty()) {
    } else {
		for (std::vector<WiseClient>::iterator item = m_clients.begin(); item != m_clients.end(); ++item) {
			item->printSensorInfo ();
		}
	}
	pthread_mutex_unlock (&lock.mutex);
}

void
WiseClientHandler::clentDataBaseInit () {
	char 	query[512];
	uint8_t addr[5];
	
	memset (query, 0x0, 512);
	sprintf (query, "SELECT SI.sensor_id, SI.sensor_hub_address, SI.sensor_address, SI.sensor_family_type, SD.value AS 'hw_value', SA.action AS 'ui_value', SI.available "
					"FROM `wiseup`.`sensor-info` SI "
					"LEFT JOIN `wiseup`.`sensor-data` SD ON SI.sensor_id = SD.sensor_id "
					"LEFT JOIN `wiseup`.`sensor-action` SA ON SI.sensor_id = SA.sensor_id");
	
	m_dbconn->executeQuery(query);
	unsigned int fieldsCount = m_dbconn->getFieldsCount ();
	
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(m_dbconn->mySQLResult))) {
		SensorInfo sensor;
		
		sensor.info.sensorAddress 	 	= atoll(row[0]);
		sensor.info.hubAddress 		 	= atoll(row[1]);
		sensor.info.sensorPort 		 	= atoll(row[2]);
		sensor.info.sensorType 		 	= atoll(row[3]);
		sensor.info.value.sensorHWValue = atoll(row[4]);
		sensor.info.value.sensorUIValue = (row[5] != NULL) ? atoll(row[5]) : sensor.info.value.sensorHWValue;
		sensor.info.flags.isAvalibale  	= 0;
		sensor.info.flags.isValueCng 	= NO;
		sensor.info.flags.isEvent		= NO;
		sensor.info.lastUpdate 		 	= 0;
		
		addr[4] = (sensor.info.hubAddress & 0x000000FF00000000) >> 32;
        addr[3] = (sensor.info.hubAddress & 0x00000000FF000000) >> 24;
		addr[2] = (sensor.info.hubAddress & 0x0000000000FF0000) >> 16;
		addr[1] = (sensor.info.hubAddress & 0x000000000000FF00) >> 8;
		addr[0] = (sensor.info.hubAddress & 0x00000000000000FF) >> 0;
		
		WiseClient* device = findClient (addr);
		if (device != NULL) {
			device->addSensor (sensor);
		} else {
			WiseClient client = WiseClient(addr);
			client.status = DISCOVERY;
			client.addSensor (sensor);
			
			pthread_mutex_lock (&lock.mutex);
			m_clients.push_back (client);
			pthread_mutex_unlock (&lock.mutex);
		}
	}
	
	m_dbconn->freeRes();
}

void
WiseClientHandler::updateSensorInfo (rfcomm_data* wisePacket) {
	uint8_t* data_ptr = wisePacket->data_frame.unframeneted.data;
	
	/* Parse the message */
	long long sensor_address = 0;
	long long hub_address 	 = 0;
	int data 				 = 0;
	
	memcpy (&hub_address, wisePacket->sender, 5);
	WiseClient* device = findClient (wisePacket->sender);
	SensorInfo* sensor = NULL;
	
	switch (wisePacket->sender_information.sender_type) {
		case SENDER_SENSOR_LOCAL_HUB: {
			rfcomm_sensor_info* sensor_info = (rfcomm_sensor_info *)data_ptr;
			while (wisePacket->data_information.data_size) {
				data_ptr += SENSOR_INFO_DATA_SIZE;
				data 	 = *data_ptr;
				data_ptr += sensor_info->sensor_data_len;
				wisePacket->data_information.data_size = wisePacket->data_information.data_size - 
											(SENSOR_INFO_DATA_SIZE + sensor_info->sensor_data_len);

				sensor_address = 0;
				memcpy (&sensor_address, wisePacket->sender, 5);
				sensor_address = (sensor_address << 8) | sensor_info->sensor_address;
				
				sensor = device->findSensor (sensor_address);
				if (sensor != NULL) {
					sensor->info.flags.isAvalibale 		= YES;
					sensor->info.lastUpdate 			= CommonMethods::getTimestampMillis();
					sensor->info.backup.sensorHWValue 	= sensor->info.value.sensorHWValue;
					sensor->info.backup.sensorUIValue 	= sensor->info.value.sensorUIValue;
					sensor->info.value.sensorHWValue 	= data;
					
					if (sensor->info.backup.sensorHWValue != sensor->info.value.sensorHWValue) {
						printf ("(WiseClientHandler) [updateSensorInfo] CHANGES (%d) -> (%d)\n", sensor->info.backup.sensorHWValue,
																								sensor->info.value.sensorHWValue);
						// TODO - Connect to event management system
						// TODO - Update local database. (MySql)
					}
					
					sensor = NULL;
				} else {
					printf ("(WiseClientHandler) [updateSensorInfo] ERROR - CAN'T FIND SENSOR (SENDER_SENSOR_LOCAL_HUB)\n");
				}
				
				sensor_info = (rfcomm_sensor_info *)data_ptr;
			}
		}
		break;
		case SENDER_SENSOR_WIRELESS_HUB: {
			rfcomm_individual_sensor_info * sensor_info = (rfcomm_individual_sensor_info *)data_ptr;
			while (wisePacket->data_information.data_size) {
				data_ptr += sizeof(rfcomm_individual_sensor_info);
				data 	 = *data_ptr;
				data_ptr += sensor_info->sensor_data_len;
				wisePacket->data_information.data_size = wisePacket->data_information.data_size - 
											sensor_info->sensor_data_len;
		
				sensor_address = 0;
				memcpy (&sensor_address, sensor_info->sensor_address, 6);
				
				sensor = device->findSensor (sensor_address);
				if (sensor != NULL) {
					sensor->info.flags.isAvalibale 		= YES;
					sensor->info.lastUpdate 			= CommonMethods::getTimestampMillis();
					sensor->info.backup.sensorHWValue 	= sensor->info.value.sensorHWValue;
					sensor->info.backup.sensorUIValue 	= sensor->info.value.sensorUIValue;
					sensor->info.value.sensorHWValue 	= data;
					
					if (sensor->info.backup.sensorHWValue != sensor->info.value.sensorHWValue) {
						printf ("(WiseClientHandler) [updateSensorInfo] CHANGES (%d) -> (%d)\n", sensor->info.backup.sensorHWValue,
																								sensor->info.value.sensorHWValue);
						// TODO - Connect to event management system
						// TODO - Update local database. (MySql)
					}
					
					sensor = NULL;
				} else {
					printf ("(WiseClientHandler) [updateSensorInfo] ERROR - CAN'T FIND SENSOR (SENDER_SENSOR_WIRELESS_HUB)\n");
				}
			}
		}
		break;
	}
	
	
	
	// printClentInfo ();
}

SensorInfo* 
WiseClientHandler::findSensor (long long sensorAddr) {
	SensorInfo* sensor = NULL;   
	pthread_mutex_lock (&lock.mutex);
    if (m_clients.empty()) {
    } else {
        /* Lets try to find this device in the list */
        for (std::vector<WiseClient>::iterator item = m_clients.begin(); item != m_clients.end(); ++item) {
			sensor = item->findSensor (sensorAddr);
            if (sensor != NULL) {
				pthread_mutex_unlock (&lock.mutex);
                return sensor;
            }
        }
    }
	
    pthread_mutex_unlock (&lock.mutex);
    return NULL;
}


bool
WiseClientHandler::updateSensorUIValue (long long sensorAddr, uint16_t uiValue) {
	SensorInfo* sensor = findSensor (sensorAddr);
	if (sensor != NULL) {
		sensor->info.value.sensorUIValue = uiValue;
	}
}

wise_status_t
WiseClientHandler::registrationCheck (rfcomm_data* wisePacket) {
    WiseClient* device = findClient (wisePacket->sender);
	
    if (wisePacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
        rfcomm_device_prot* prot = (rfcomm_device_prot*)wisePacket->data_frame.unframeneted.data;
		printf ("DEBUG\n");
        
        if (prot->device_cmd == DEVICE_PROT_CONNECT_REQ) {
			printf ("DEBUG_0\n");
            if (device != NULL) {
                /* Update timestamp */
                device->timestamp = (uint64_t)time(NULL);
				printf ("DEBUG_1\n");
                
                if (device->status == DISCOVERY) {
					printf ("DEBUG_2\n");
                    return DISCOVERY;
                } else if (device->status == CONNECTED) {
					printf ("DEBUG_3\n");
					WiseDBMng::apiSetSensorAvailability (wisePacket, true); // Set HUB as available
                    return CONNECTED;
                }
            } else {
				//printf ("DEBUG_6\n");
                /* 
                 * New device, let us add it to the list of devices and
                 * send our address back to the device. When the device will recieve 
                 * our address it will stop broadcasting and know this gateway.
                 */
                printf ("(WiseClientHandler) [registrationCheck] Adding new device \n");
                WiseClient client   = WiseClient(wisePacket->sender);
                client.timestamp    = (uint64_t)time(NULL);
                client.status       = DISCOVERY;
                m_clients.push_back (client);
				
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
WiseClientHandler::sendRegistration (rfcomm_data* wisePacketRX) {
    rfcomm_data wisePacketTX;

    /* Create pacakge */
    wisePacketTX.data_information.data_type      = DEVICE_PROT_DATA_TYPE;
    wisePacketTX.data_information.data_size      = DEVICE_PROT_CONN_DATA_SIZE;
    wisePacketTX.control_flags.is_fragmeneted    = 0;
    wisePacketTX.control_flags.version           = 1;
    wisePacketTX.control_flags.is_broadcast      = 0;
    wisePacketTX.control_flags.is_ack            = 0;
    wisePacketTX.magic_number[0]                 = 0xAA;
    wisePacketTX.magic_number[1]                 = 0xBB;
    memcpy (wisePacketTX.sender, local_address, 5);
    memcpy (wisePacketTX.target, wisePacketRX->sender, 5);
    rfcomm_device_prot* deviceProt              = (rfcomm_device_prot *)wisePacketTX.data_frame.unframeneted.data;
    deviceProt->device_cmd                      = DEVICE_PROT_CONNECT_ADDR;
    memcpy (deviceProt->device_data, local_address, 5);

    /* Send to OUTGOING queue */
    WiseIPC *ipcPacketsOut = new WiseIPC ("/tmp/wiseup/nrf_outgoing_queue");
    if (ipcPacketsOut->setClient () == SUCCESS) {
        ipcPacketsOut->setBuffer((unsigned char *)&wisePacketTX);
        if (ipcPacketsOut->sendMsg(32) == false) { }
        else { CommonMethods::printBuffer ("(WiseClientHandler) [sendRegistration] Registration for ", wisePacketTX.target, 5); }
    }

    delete ipcPacketsOut;
}

void
WiseClientHandler::removeUnusedDeveices () {
	pthread_mutex_lock (&lock.mutex);
    for (std::vector<WiseClient>::iterator item = m_clients.begin(); item != m_clients.end(); ++item) {
        if ( (uint64_t)time(NULL) - item->timestamp > 60) {
            printf ("(WiseClientHandler) [removeUnusedDeveices] ... \n");
            item->printAddress();
            m_clients.erase (item);
			pthread_mutex_unlock (&lock.mutex);
            return;
        }
    }
	
	pthread_mutex_unlock (&lock.mutex);
}

WiseClient* 
WiseClientHandler::findClient (uint8_t * address) {
    WiseClient client = WiseClient(address);
    
	pthread_mutex_lock (&lock.mutex);
    if (m_clients.empty()) {
    } else {
        /* Lets try to find this device in the list */
        for (std::vector<WiseClient>::iterator item = m_clients.begin(); item != m_clients.end(); ++item) {
            if (*item == client) {
				pthread_mutex_unlock (&lock.mutex);
                return &(*item);
            }
        }
    }
	
    pthread_mutex_unlock (&lock.mutex);
    return NULL;
}

WiseClient::WiseClient (uint8_t * addr) {
	memcpy (address, addr, 5);
}

WiseClient::~WiseClient () {
}

void
WiseClient::addSensor (SensorInfo &info) {
	sensors.push_back (info);
}

bool
WiseClient::removeSensor (long long id) {
	if (sensors.empty()) {
    } else {
		for (std::vector<SensorInfo>::iterator item = sensors.begin(); item != sensors.end(); ++item) {
			if (item->info.sensorAddress == id) {
				sensors.erase (item);
				return true;
			}
		}
	}
	
	return false;
}

SensorInfo *
WiseClient::findSensor (long long id) {
    if (sensors.empty()) {
    } else {
        /* Lets try to find this device in the list */
        for (std::vector<SensorInfo>::iterator item = sensors.begin(); item != sensors.end(); ++item) {
            if (item->info.sensorAddress == id) {
                return &(*item);
            }
        }
    }
    
    return NULL;
}

int
WiseClient::countSensor () {
	return sensors.size();
}

void
WiseClient::printSensorInfo () {
	for (std::vector<SensorInfo>::iterator sensor = sensors.begin(); sensor != sensors.end(); ++sensor) {
		printf ("%lld, %lld, %d, %d, %d, %d, %d, %lld\n", sensor->info.sensorAddress, 
													sensor->info.hubAddress,
													sensor->info.sensorPort, 
													sensor->info.sensorType,
													sensor->info.value.sensorHWValue, 
													sensor->info.value.sensorUIValue, 
													sensor->info.flags.isAvalibale,
													sensor->info.lastUpdate);
	}
}

WiseCommandHandler::WiseCommandHandler () {
}

WiseCommandHandler::~WiseCommandHandler () {
}

void
WiseCommandHandler::commandHandler (rfcomm_data* wisePacket) {    
    if (wisePacket->data_information.data_type == DEVICE_PROT_DATA_TYPE) {
        rfcomm_device_prot* prot = (rfcomm_device_prot*)wisePacket->data_frame.unframeneted.data;
        
        switch (prot->device_cmd) {
            case 0:
                
            break;
        }
    }
}

SensorInfo::SensorInfo () {
}

SensorInfo::~SensorInfo () {
}
	
void
SensorInfo::addEvent (sensor_event_t &event) {	
	events.push_back (event);
}

bool
SensorInfo::removeEvent (uint32_t id) {
	for (std::vector<sensor_event_t>::iterator item = events.begin(); item != events.end(); ++item) {
        if (item->eventId == id) {
            events.erase (item);
            return true;
        }
    }
	
	return false;
}

sensor_event_t*
SensorInfo::findEvent (uint32_t id) {	
	if (events.empty()) {
    } else {
        /* Lets try to find this event in the list */
        for (std::vector<sensor_event_t>::iterator item = events.begin(); item != events.end(); ++item) {
            if (item->eventId == id) {
                return &(*item);
            }
        }
    }
    
    return NULL;
}

int
SensorInfo::getEventSize (){
	return events.size();
}
