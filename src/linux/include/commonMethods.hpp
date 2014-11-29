/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Wiseup.
 */

#pragma once

#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <sys/time.h>
#include "wise_ipc.h"

#define YES 	1
#define NO 		0

typedef struct {
	pthread_cond_t  cond;
	pthread_mutex_t mutex;
} sync_context_t;

typedef struct {
    uint32_t txPacketCount;
    uint32_t rxPacketCount;
    uint8_t  cpuTemperature;
    uint8_t  cpuUsage;
} screen_context;

typedef struct {

} deamon_context;

typedef struct { 
    uint8_t isAvalibale : 1;
    uint8_t isEvent     : 1;
    uint8_t isValueCng  : 1;
    uint8_t reserved    : 5;
} sensor_control_t;

typedef struct { 
    uint16_t            sensorHWValue;
    uint16_t            sensorUIValue;
} sensor_value_t;

typedef struct { 
    long long           sensorAddress;
    long long           hubAddress;
    uint8_t             sensorPort;
    uint8_t             sensorType;
    sensor_value_t      value;
    sensor_value_t      backup;
    uint64_t            lastUpdate;
    sensor_control_t    flags;
    uint16_t            updateInterval;
} sensor_info_t;

class CommonMethods {
public:
    static uint64_t getTimestampMillis () {
        struct timeval tv;

        gettimeofday(&tv, NULL);

        return (uint64_t)(1000000 * tv.tv_sec + tv.tv_usec);
    }

    static void printBuffer (char* name, uint8_t* buff, int len) {
    	printf ("%s ", name);
	    for (int i = 0; i < len; i++) {
	        printf ("%x ", buff[i]);
	    }
	    printf ("\n");
	}

    void stdout_msg () {
        /* Initiate UNIX socket as stdout */
        /*WiseIPC *ipcSTDOUT = new WiseIPC ("/tmp/wiseup/stdout_pipe");
        if (ipcSTDOUT->setClient () == SUCCESS) {
            ipcSTDOUT->setBuffer (wise_stdout_buffer);
            if (ipcSTDOUT->sendMsg(128) == false) { }
        }

        delete ipcSTDOUT;*/
    }
};