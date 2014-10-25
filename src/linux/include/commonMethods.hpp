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

#define SP_UPDATE_SENSOR_INFO 				1
#define SP_SET_SENSOR_AVAILABILITY			2
#define SP_SET_ALL_SENSOR_NOT_CONNECTED		3

typedef struct {
    uint16_t    magic; 
    uint8_t     packet[32];
    uint64_t    timestamp;
	bool		isGeneratePacketID;
} nrf24l01_msg_t;

typedef struct {
	uint8_t		spId;
	uint8_t		args[16];
    rfcomm_data packet;
    uint64_t    timestamp;
} db_msg_t;

typedef struct {
        pthread_cond_t  cond;
        pthread_mutex_t mutex;
} sync_context_t;

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