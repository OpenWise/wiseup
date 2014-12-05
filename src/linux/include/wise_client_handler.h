/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <cstdlib>
#include <cstring>
#include <stdlib.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <stdio.h>

#include "wiseDBMng.h"
#include "wise_mysql.h"

using namespace std;

typedef enum {
    DISCOVERY       =   0,
    CONNECTED       =   1,
    UNKNOWN         =   99
} wise_status_t;

typedef struct {
	uint32_t	eventId;
	uint8_t		eventType;
} sensor_event_t;

class SensorInfo {
public:
	SensorInfo ();
	~SensorInfo ();
	
	void 				addEvent (sensor_event_t &event);
	bool 				removeEvent (uint32_t id);
	sensor_event_t*		findEvent (uint32_t id);
	int 				getEventSize ();
	
	sensor_info_t			info;
	vector<uint64_t>		ApplicationIDList;
	vector<sensor_event_t> 	events;
};

class WiseClient {
    public:
		WiseClient (uint8_t * addr);
		~WiseClient ();
		
        friend bool operator== ( const WiseClient &wc1, const WiseClient &wc2 ) {
            for (int i = 0; i < 5; i++) {
                if (wc1.address[i] != wc2.address[i])
                    return false;
            }
            return true;
        }
        
        void printAddress () {
            printf ("[ ");
            for (int i = 0; i < 5; i++) {
                printf ("%x ", address[i]);
            } printf ("]\n");
        }
		
		void 		addSensor (SensorInfo &info);
		bool 		removeSensor (long long id);
		SensorInfo* findSensor (long long id);
		SensorInfo* popSensor ();
		int 		countSensor ();
		void		printSensorInfo ();
        
        uint8_t         	address[5];
        uint64_t        	timestamp;
        wise_status_t   	status;
		vector<SensorInfo> 	sensors;
};

class WiseClientHandler {
    public:
        WiseClientHandler ();
        ~WiseClientHandler ();

        /*
         * Need to update this method with stl find method.
         */
        wise_status_t   registrationCheck (rfcomm_data* wisePacket);
		void            sendRegistration (rfcomm_data* wisePacket);
		void			addNewClient (uint8_t* address);
		void			addNewClientSensors (rfcomm_data* wisePacket);
		
		void			disableSensorUI (long long sensorID);
        void            removeUnusedDeveices ();
        WiseClient*     findClient (uint8_t * address);
        
		void			clentDataBaseInit ();
		void			updateSensorInfo (rfcomm_data* wisePacket);
		SensorInfo* 	findSensor (long long sensorAddr);
		bool			updateSensorUIValue (long long sensorAddr, uint16_t uiValue);
		void			printClentInfo ();
		
		sync_context_t	lock;

    private:
        /* List of WiseUp clients */
        vector<WiseClient>  m_clients;
		MySQL *				m_dbconn;
};

class WiseCommandHandler {
    public:
        WiseCommandHandler ();
        ~WiseCommandHandler ();

        void    commandHandler (rfcomm_data* wisePacket);
};
