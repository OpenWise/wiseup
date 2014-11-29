/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */

#pragma once

#include <iostream>
#include <pthread.h>
#include <vector>
#include <stdlib.h>

/* includes for wiseup */
#include "wise_mysql.h"
#include "wise_rfcomm.hpp"
#include "wise_ipc.h"
#include "commonMethods.hpp"

using namespace std;

#define SP_UPDATE_SENSOR_INFO_RAW_DATA 		1
#define SP_SET_SENSOR_AVAILABILITY			2
#define SP_SET_ALL_SENSOR_NOT_CONNECTED		3
#define SP_UPDATE_SENSOR_INFO_DATA 			4
#define SP_SET_HUB_SENSORS_AVAILABILITY		5

typedef struct {
	uint8_t		spId;
	uint8_t		args[32];
    rfcomm_data packet;
    uint64_t    timestamp;
} db_msg_t;

typedef struct {
	long long 	address;
	uint8_t 	id;
	long long 	hubAddress;
	uint8_t 	type;
	uint16_t 	value;
	bool 		availability;
	uint16_t	updateInterval;
} db_sensor_info_t;

class WiseDBDAL {
public:
	WiseDBDAL ();
	~WiseDBDAL ();
	
	void updateSensorInfo (long long sensorAddres, long long hubAddress, uint8_t sensorPort, 
							uint8_t sensorType, bool availability, uint16_t value, uint16_t updateInterval);
	void setSensorAvailability (long long sensorAddress, bool availability);
	void setHubSensorsAvailability (long long hubAddress, bool availability);
	void setAllSensorNotConnected ();
	
private:
	MySQL *	m_dbconn;
};

class WiseDBMng {
public:
	WiseDBMng (WiseDBDAL* db);
	~WiseDBMng ();
	
	static void apiUpdateSensorsInfo (rfcomm_data * data);
	static void apiUpdateSensorInfo (long long address, uint8_t id, long long hubAddress, uint8_t type, uint16_t value, uint16_t updateInterval);
	static void apiSetSensorAvailability (long long address, bool availability);
	static void apiSetHubSensorsAvailability (rfcomm_data * data, bool availability);
	static void apiSetAllSensorNotConnected ();
	
	bool start ();
	void stop ();
	
	bool				m_isWorking;
	sync_context_t		m_lock;
	WiseDBDAL* 			m_Dal;

private:
	pthread_t       	m_worker;
};
