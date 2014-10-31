/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */

/*
 * TODO - Make a template for the vector management. (code reuse)
 */

#include <iostream>
#include "wiseCacheDB.h"
#include "wiseEventMng.h"

using namespace std;

void * 
cacheDBWorker (void * args) {
	cache_db_msg_t  msg;
	WiseIPC* ipcDB  = NULL;
	CacheDB* obj 	= (CacheDB*)args;
	
	int client	= -1;
	ipcDB 		= new WiseIPC ("/tmp/wiseup/cache_db_pipe");
	ipcDB->setServer ();
    ipcDB->setBuffer ((unsigned char *)&msg);
	
	long long sensorAddress = 0;
	long long hubAddress 	= 0;
	
	rfcomm_data* wisePacket = (rfcomm_data *)&msg.packet;
	while (obj->m_isWorking) {
		client = ipcDB->listenIPC ();
		memset (ipcDB->buff, 0, sizeof(cache_db_msg_t));
        
        read (client, ipcDB->buff, sizeof(cache_db_msg_t));
		switch (msg.requestId) {
			case CACHE_DB_UPDATE_SENSORS_VALUE_FROM_RFCOMM_DATA: {
				obj->updateSensorsValueFromRfcommDataHandler (msg);
			}
			break;
		}
		close (client);
	}
	
	delete ipcDB;
}

void
CacheDB::printSensorsInfo () {
	for (std::vector<SensorInfo>::iterator item = m_db.begin(); item != m_db.end(); ++item) {
		printf ("%lld, %lld, %d, %d, %d, %d, %d\n", item->sensorInfo.sensorAddress, 
													item->sensorInfo.hubAddress,
													item->sensorInfo.sensorPort, 
													item->sensorInfo.sensorType,
													item->sensorInfo.value.sensorHWValue, 
													item->sensorInfo.value.sensorUIValue, 
													item->sensorInfo.flags.isAvalibale);
	}
}

void
CacheDB::initDB () {
	char query[512];
	
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
		
		sensor.sensorInfo.sensorAddress 	 	= atoll(row[0]);
		sensor.sensorInfo.hubAddress 		 	= atoll(row[1]);
		sensor.sensorInfo.sensorPort 		 	= atoll(row[2]);
		sensor.sensorInfo.sensorType 		 	= atoll(row[3]);
		sensor.sensorInfo.value.sensorHWValue 	= atoll(row[4]);
		sensor.sensorInfo.value.sensorUIValue 	= (row[5] != NULL) ? atoll(row[5]) : sensor.sensorInfo.value.sensorHWValue;
		sensor.sensorInfo.flags.isAvalibale  	= atoll(row[6]);
		sensor.sensorInfo.flags.isValueCng 		= NO;
		sensor.sensorInfo.flags.isEvent			= NO;
		sensor.sensorInfo.lastUpdate 		 	= 0;
		
		add (sensor);
	}
	
	m_dbconn->freeRes();
	printSensorsInfo ();
}

CacheDB::CacheDB () {
	struct DBCredentials cred;
	
    cred.hostName   = "localhost";
    cred.userId     = "root";
    cred.password   = "sql123";
    cred.database   = "wiseup";
	
	m_dbconn = new MySQL();
	m_dbconn->setupConnection(&cred);
	
	initDB ();
}

CacheDB::~CacheDB () {
	delete m_dbconn;
}

bool
CacheDB::start () {
	m_isWorking = true;
	int error 	= pthread_create(&m_worker, NULL, cacheDBWorker, this);
    if (error) {
         return false;
    }

    return true;
}

void
CacheDB::stop () {
	m_isWorking = false;
	pthread_cancel (m_worker);
}

void
CacheDB::add (SensorInfo &info) {
	pthread_mutex_lock (&m_lock.mutex);
	m_db.push_back (info);
	pthread_mutex_unlock (&m_lock.mutex);
}

bool
CacheDB::remove (long long id) {
	pthread_mutex_lock (&m_lock.mutex);
	for (std::vector<SensorInfo>::iterator item = m_db.begin(); item != m_db.end(); ++item) {
        if (item->sensorInfo.sensorAddress == id) {
            m_db.erase (item);
			pthread_mutex_unlock (&m_lock.mutex);
            return true;
        }
    }
	
	pthread_mutex_unlock (&m_lock.mutex);
	return false;
}

bool
CacheDB::find (long long id, SensorInfo& data) {
	unsigned int counter = 0;
	pthread_mutex_lock (&m_lock.mutex);
	for (std::vector<SensorInfo>::iterator item = m_db.begin(); item != m_db.end(); ++item) {
        if (item->sensorInfo.sensorAddress == id) {
			pthread_mutex_unlock (&m_lock.mutex);
			data = m_db.at(counter);
            return true;
        }
		counter++;
    }
	
	pthread_mutex_unlock (&m_lock.mutex);
	return false;
}

int
CacheDB::getSize () {
	return m_db.size();
}

void
CacheDB::apiUpdateSensorsValueFromRfcommData (rfcomm_data* data) {
	WiseIPC 		*ipcCacheDB = new WiseIPC ("/tmp/wiseup/cache_db_pipe");
	cache_db_msg_t	msg;
	
	if (ipcCacheDB->setClient () == SUCCESS) {
		memcpy(&msg.packet, data, 32);
		ipcCacheDB->setBuffer((unsigned char *)&msg);
		msg.requestId 		= CACHE_DB_UPDATE_SENSORS_VALUE_FROM_RFCOMM_DATA;
		msg.requestorType 	= HW_REQUESTOR;
		if (ipcCacheDB->sendMsg(sizeof(cache_db_msg_t)) == false) { }
	} else {
		printf ("(CacheDB) [ERROR] - No available db_pipe \n");
	}
	
	delete ipcCacheDB;
}

void
CacheDB::updateSensorsValueFromRfcommDataHandler (cache_db_msg_t& data) {
	printf ("(CacheDB) [updateSensorsValueHandler] ... \n");
	long long 	sensor_address	= 0;
	long long 	hub_address 	= 0;
	int 		sensorData	 	= 0;
	bool		dbChanged		= false;
	
	rfcomm_data*		 wisePacket 	= (rfcomm_data *)&data.packet;
	uint8_t* 			 data_ptr 		= wisePacket->data_frame.unframeneted.data;
	rfcomm_sensor_info*  sensor_info	= (rfcomm_sensor_info *)data_ptr;
	SensorInfo			 sensor;
	
	while (wisePacket->data_information.data_size) {
		data_ptr += SENSOR_INFO_DATA_SIZE;
		sensorData = *data_ptr;
		data_ptr += sensor_info->sensor_data_len;
		wisePacket->data_information.data_size = wisePacket->data_information.data_size - 
									(SENSOR_INFO_DATA_SIZE + sensor_info->sensor_data_len);

		sensor_address = 0;
		memcpy (&sensor_address, wisePacket->sender, 5);
		sensor_address = (sensor_address << 8) | sensor_info->sensor_address;
		memcpy (&hub_address, wisePacket->sender, 5);
		
		if (find (sensor_address, sensor)) {
			sensor.sensorInfo.backup.sensorHWValue = sensor.sensorInfo.value.sensorHWValue;
			sensor.sensorInfo.backup.sensorUIValue = sensor.sensorInfo.value.sensorUIValue;
		
			sensor.sensorInfo.value.sensorHWValue = sensorData;
			if (sensor.sensorInfo.sensorType != 4) {
				sensor.sensorInfo.value.sensorUIValue = sensorData;
			}
			sensor.sensorInfo.lastUpdate = CommonMethods::getTimestampMillis();
			
			if (sensor.sensorInfo.backup.sensorHWValue != sensor.sensorInfo.value.sensorHWValue) {
				sensor.sensorInfo.flags.isValueCng = YES;
				dbChanged = true;
			}
		} else {
			// TODO - Add new sensor to the cache DB
			SensorInfo newSensor;
		
			newSensor.sensorInfo.sensorAddress 	 		= sensor_address;
			newSensor.sensorInfo.hubAddress 			= hub_address;
			newSensor.sensorInfo.sensorPort 			= sensor_info->sensor_address;
			newSensor.sensorInfo.sensorType 			= sensor_info->sensor_type;
			newSensor.sensorInfo.value.sensorHWValue	= sensorData;
			newSensor.sensorInfo.value.sensorUIValue 	= sensorData;
			newSensor.sensorInfo.flags.isAvalibale  	= YES;
			sensor.sensorInfo.flags.isValueCng 			= NO;
			sensor.sensorInfo.flags.isEvent				= NO;
			newSensor.sensorInfo.lastUpdate 			= CommonMethods::getTimestampMillis();
			
			add (newSensor);
			
			printf ("(CacheDB) [updateSensorsValueHandler] Adding new sensor\n");
		}
		
		sensor_info = (rfcomm_sensor_info *)data_ptr;
	}
	
	if (dbChanged == true) {
		EventMng::apiAlertDBChanges ();
	}
}

void
CacheDB::updateSensorValue (cache_db_msg_t& data) {
	
}

void
CacheDB::updateSensorAvailable (cache_db_msg_t& data) {
	cache_db_available_arg_t* availablePtr = (cache_db_available_arg_t*)data.args;
	bool available = (availablePtr->available == 1) ? true : false;
}

void
CacheDB::addSensorEvent (cache_db_msg_t& data) {

}

void
CacheDB::removeSensorEvent (cache_db_msg_t& data) {

}

SensorInfo::SensorInfo () {

}

SensorInfo::~SensorInfo () {

}
	
void
SensorInfo::addEvent (sensor_event_t &event) {	
	sensorEvents.push_back (event);
}

bool
SensorInfo::removeEvent (uint32_t id) {
	for (std::vector<sensor_event_t>::iterator item = sensorEvents.begin(); item != sensorEvents.end(); ++item) {
        if (item->eventId == id) {
            sensorEvents.erase (item);
            return true;
        }
    }
	
	return false;
}

bool
SensorInfo::findEvent (uint32_t id, sensor_event_t& event) {
	for (std::vector<sensor_event_t>::iterator item = sensorEvents.begin(); item != sensorEvents.end(); ++item) {
        if (item->eventId == id) {
			event = *item;
            return true;
        }
    }
	
	return false;
}

int
SensorInfo::getEventSize (){
	return sensorEvents.size();
}
