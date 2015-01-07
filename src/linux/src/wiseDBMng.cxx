/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */
 
#include <iostream>
#include "wiseDBMng.h"

using namespace std;

WiseDBDAL::WiseDBDAL () {
}

WiseDBDAL::~WiseDBDAL () {
}

void
WiseDBDAL::updateSensorInfo (long long sensorAddres, long long hubAddress, uint8_t sensorPort, 
						uint8_t sensorType, bool availability, uint16_t value, uint16_t updateInterval) {
	/*char query[256];
	memset (query, 0x0, 256);
	sprintf (query, "call sp_update_sensor_info ('%lld', '%lld', '%d', '%d', '%d', '%d', '%d')", 
				sensorAddres, hubAddress, sensorPort, sensorType, (availability == true) ? 1:0, value, updateInterval);

	m_dbconn->executeQuery(query);
	m_dbconn->freeRes();*/
}

void
WiseDBDAL::setSensorAvailability (long long sensorAddress, bool availability) {
	/*char query[256];
	memset (query, 0x0, 256);
	sprintf (query, "call sp_set_sensor_availability ('%lld', '%d')", sensorAddress, (availability == true) ? 1:0);

	m_dbconn->executeQuery(query);
	m_dbconn->freeRes();*/
}

void
WiseDBDAL::setHubSensorsAvailability (long long hubAddress, bool availability) {
	/*char query[256];
	memset (query, 0x0, 256);
	sprintf (query, "call sp_set_hub_sensors_availability ('%lld', '%d')", hubAddress, (availability == true) ? 1:0);

	m_dbconn->executeQuery(query);
	m_dbconn->freeRes();*/
}

void
WiseDBDAL::setAllSensorNotConnected () {
	/*char query[256];
	memset (query, 0x0, 256);
	sprintf (query, "call sp_set_all_sensor_not_connected ()");

	m_dbconn->executeQuery(query);
	m_dbconn->freeRes();*/
}

void * 
nodeJSWorker (void * args) {
	unsigned char buffer[512];
	WiseIPC *ipcNodeJS = new WiseIPC ("/tmp/wiseup/nodejs_pipe");
	
	WiseDBMng* obj 	= (WiseDBMng*)args;
	int client		= -1;
	
	ipcNodeJS->setServer ();
    ipcNodeJS->setBuffer (buffer);
	while (obj->m_isNodeJSWorking) {
		if (client == -1) {
			client = ipcNodeJS->listenIPC ();
			printf ("#(wiseDBMng) NODE.JS CONNECTED\n");
		} else {
			ssize_t dataLength = read (client, ipcNodeJS->buff, 512);
			if (dataLength > 0) {
				printf ("#(wiseDBMng) Data from Node.js (%s)\n", buffer);
				write(client, buffer, 512);
			} else if (dataLength < 0) {
				printf ("#(wiseDBMng) ERROR\n");
			}
		}
	}
	
	close (client);
}

void * 
dbMngWorker (void * args) {
	db_msg_t 			msg;
	WiseIPC 			*ipcDB  		= NULL;
	rfcomm_data 		*wisePacket 	= NULL;
	
	WiseDBMng* obj 	= (WiseDBMng*)args;
	
	int client	= -1;
	ipcDB 		= new WiseIPC ("/tmp/wiseup/db_pipe");
	ipcDB->setServer ();
    ipcDB->setBuffer ((unsigned char *)&msg);
	
	long long sensorAddress = 0;
	long long hubAddress 	= 0;
	
	wisePacket = (rfcomm_data *)&msg.packet;
    while (obj->m_isWorking) {
        client = ipcDB->listenIPC ();
		memset (ipcDB->buff, 0, sizeof(db_msg_t));
        read (client, ipcDB->buff, sizeof(db_msg_t));
		
		switch (msg.spId) {
			case SP_UPDATE_SENSOR_INFO_RAW_DATA: {
				uint8_t* data_ptr = wisePacket->data_frame.unframeneted.data;
				
				printf ("(WiseDBMng) [dbMngWorker] SP_UPDATE_SENSOR_INFO (%d)\n", wisePacket->sender_information.sender_type);
				switch (wisePacket->sender_information.sender_type) {
					case SENDER_SENSOR_LOCAL_HUB: {
						printf ("(WiseDBMng) [dbMngWorker] SP_UPDATE_SENSOR_INFO - SENDER_SENSOR_LOCAL_HUB\n");
						rfcomm_sensor_info * sensor_info = (rfcomm_sensor_info *)data_ptr;

						/* Parse the message */
						long long sensor_address = 0;
						long long hub_address 	 = 0;
						int data 				 = 0;
						
						while (wisePacket->data_information.data_size) {
							data_ptr += sizeof(rfcomm_sensor_info);
							data 	 = *data_ptr;
							data_ptr += sensor_info->sensor_data_len;
							wisePacket->data_information.data_size = wisePacket->data_information.data_size - 
														(sizeof(rfcomm_sensor_info) + sensor_info->sensor_data_len);
					
							sensor_address = 0;
							memcpy (&sensor_address, wisePacket->sender, 5);
							sensor_address = (sensor_address << 8) | sensor_info->sensor_address;
							memcpy (&hub_address, wisePacket->sender, 5);
							
							// Calling DAL methods
							obj->m_Dal->updateSensorInfo (sensor_address, hub_address, sensor_info->sensor_address, 
																sensor_info->sensor_type, true, data, sensor_info->sensor_update_interval);
																
							sensor_info = (rfcomm_sensor_info *)data_ptr;
						}
					}
					break;
					case SENDER_SENSOR_WIRELESS_HUB: {
						printf ("(WiseDBMng) [dbMngWorker] SP_UPDATE_SENSOR_INFO - SENDER_SENSOR_WIRELESS_HUB\n");
						rfcomm_individual_sensor_info * sensor_info = (rfcomm_individual_sensor_info *)data_ptr;

						/* Parse the message */
						long long sensor_address = 0;
						long long hub_address 	 = 0;
						int data 				 = 0;
						
						while (wisePacket->data_information.data_size) {
							data_ptr += sizeof(rfcomm_individual_sensor_info);
							data 	 = *data_ptr;
							data_ptr += sensor_info->sensor_data_len;
							wisePacket->data_information.data_size = wisePacket->data_information.data_size - 
														sensor_info->sensor_data_len;
					
							sensor_address = 0;
							memcpy (&sensor_address, sensor_info->sensor_address, 6);
							memcpy (&hub_address, wisePacket->sender, 5);
							
							// Calling DAL methods
							obj->m_Dal->updateSensorInfo (sensor_address, hub_address, 0, 
															sensor_info->sensor_type, true, data, sensor_info->sensor_update_interval);
																
							sensor_info = (rfcomm_individual_sensor_info *)data_ptr;
						}
					}
					break;
				}
			}
			break;
			case SP_SET_SENSOR_AVAILABILITY: {
				db_sensor_info_t* sensor = (db_sensor_info_t* )msg.args;
				
				// Calling DAL methods
				obj->m_Dal->setSensorAvailability (sensor->address, sensor->availability);
				printf ("(WiseDBMng) [dbMngWorker] SP_SET_SENSOR_AVAILABILITY (%s)\n", (sensor->availability) ? "true" : "false");
			}
			break;
			case SP_SET_HUB_SENSORS_AVAILABILITY: {
				printf ("(WiseDBMng) [dbMngWorker] SP_SET_HUB_SENSORS_AVAILABILITY (%s)\n", (msg.args[0] == 1) ? "true" : "false");
				long long hub_address = 0;
				memcpy (&hub_address, wisePacket->sender, 5);
				
				// Calling DAL methods
				obj->m_Dal->setHubSensorsAvailability (hub_address, (msg.args[0] == 1) ? true : false);
			}
			break;
			case SP_SET_ALL_SENSOR_NOT_CONNECTED: {
				printf ("(WiseDBMng) [dbMngWorker] SP_SET_ALL_SENSOR_NOT_CONNECTED\n");
				// Calling DAL methods
				obj->m_Dal->setAllSensorNotConnected ();
			}
			break;
			case SP_UPDATE_SENSOR_INFO_DATA: {
				printf ("(WiseDBMng) [dbMngWorker] SP_UPDATE_SENSOR_INFO_DATA\n");
				// Calling DAL methods
				db_sensor_info_t* sensor = (db_sensor_info_t* )msg.args;
				obj->m_Dal->updateSensorInfo (sensor->address, sensor->hubAddress, sensor->id, 
																sensor->type, true, sensor->value, sensor->updateInterval);
			}
			break;
		}
		
		close (client);
    }
	
	delete ipcDB;
}

WiseDBMng::WiseDBMng (WiseDBDAL* db) {
	m_Dal = db;
	
	m_isNodeJSWorking = true;
	int error = pthread_create(&m_nodeJSWorker, NULL, nodeJSWorker, this);
    if (error) {
        exit (-1);
    }
}

WiseDBMng::~WiseDBMng () {
}

bool
WiseDBMng::start () {
	m_isWorking = true;
	int error 	= pthread_create(&m_worker, NULL, dbMngWorker, this);
    if (error) {
         return false;
    }

    return true;
}

void
WiseDBMng::stop () {
	m_isWorking = false;
	pthread_cancel (m_worker);
}

void
WiseDBMng::apiUpdateSensorsInfo (rfcomm_data * data) {
	db_msg_t 	msg;
	WiseIPC 	*ipcDB = new WiseIPC ("/tmp/wiseup/db_pipe");
	if (ipcDB->setClient () == SUCCESS) {
		memcpy(&msg.packet, (uint8_t *)data, 32);
		ipcDB->setBuffer((unsigned char *)&msg);
		msg.spId = SP_UPDATE_SENSOR_INFO_RAW_DATA;
		if (ipcDB->sendMsg(sizeof(db_msg_t)) == false) { }
	} else {
		printf ("(wise-nrfd) [ERROR] - No available db_pipe \n");
	}
	delete ipcDB;
}

void
WiseDBMng::apiUpdateSensorInfo (long long address, uint8_t id, long long hubAddress, uint8_t type, uint16_t value, uint16_t updateInterval) {
	unsigned char messageBuffer[512];
	WiseIPC *ipcNodeJS = new WiseIPC ("/tmp/wiseup/nodejs_out_pipe");
	if (ipcNodeJS->setClient () == SUCCESS) {
		ipcNodeJS->setBuffer(messageBuffer);
		
	} else {
		printf ("#(WiseDBMng) [ERROR] - No available Node.JS service \n");
	}
	delete ipcNodeJS;
	
	/*db_msg_t 			msg;
	db_sensor_info_t* 	sensor;
	WiseIPC 	*ipcDB = new WiseIPC ("/tmp/wiseup/db_pipe");
	if (ipcDB->setClient () == SUCCESS) {
		ipcDB->setBuffer((unsigned char *)&msg);
		msg.spId = SP_UPDATE_SENSOR_INFO_DATA;

		sensor = (db_sensor_info_t* )msg.args;
		sensor->address = address;
		sensor->id = id;
		sensor->hubAddress = hubAddress;
		sensor->type = type;
		sensor->value = value;
		sensor->updateInterval = updateInterval;

		if (ipcDB->sendMsg(sizeof(db_msg_t)) == false) { }
	} else {
		printf ("(wise-nrfd) [ERROR] - No available db_pipe \n");
	}
	delete ipcDB;*/
	
	
}

void
WiseDBMng::apiSetSensorAvailability (long long address, bool availability) {
	db_msg_t 			msg;
	db_sensor_info_t* 	sensor;
	WiseIPC 	*ipcDB = new WiseIPC ("/tmp/wiseup/db_pipe");
	if (ipcDB->setClient () == SUCCESS) {
		ipcDB->setBuffer((unsigned char *)&msg);
		msg.spId = SP_SET_SENSOR_AVAILABILITY;
		
		sensor = (db_sensor_info_t* )msg.args;
		sensor->address = address;
		sensor->availability = availability;
		
		if (ipcDB->sendMsg(sizeof(db_msg_t)) == false) { }
	} else {
		printf ("(wise-nrfd) [ERROR] - No available db_pipe \n");
	}
	delete ipcDB;
}

void
WiseDBMng::apiSetHubSensorsAvailability (rfcomm_data * data, bool availability) {
	db_msg_t 	msg;
	WiseIPC 	*ipcDB = new WiseIPC ("/tmp/wiseup/db_pipe");
	if (ipcDB->setClient () == SUCCESS) {
		memcpy(&msg.packet, (uint8_t *)data, 32);
		ipcDB->setBuffer((unsigned char *)&msg);
		msg.spId = SP_SET_HUB_SENSORS_AVAILABILITY;
		msg.args[0] = (availability == true) ? YES:NO;
		if (ipcDB->sendMsg(sizeof(db_msg_t)) == false) { }
	} else {
		printf ("(wise-nrfd) [ERROR] - No available db_pipe \n");
	}
	delete ipcDB;
}

void
WiseDBMng::apiSetAllSensorNotConnected () {
	db_msg_t 	msg;
	WiseIPC 	*ipcDB = new WiseIPC ("/tmp/wiseup/db_pipe");
	if (ipcDB->setClient () == SUCCESS) {
		ipcDB->setBuffer((unsigned char *)&msg);
		msg.spId = SP_SET_ALL_SENSOR_NOT_CONNECTED;
		if (ipcDB->sendMsg(sizeof(db_msg_t)) == false) { }
	} else {
		printf ("(wise-nrfd) [ERROR] - No available db_pipe \n");
	}
	delete ipcDB;
}
	