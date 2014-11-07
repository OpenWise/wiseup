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
#include "wise_rfcomm.hpp"

#define YES 	1
#define NO 		0

#define YES 	1
#define NO 		0

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