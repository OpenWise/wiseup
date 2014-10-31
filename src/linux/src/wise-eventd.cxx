/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <unistd.h>
#include <iostream>
#include <stdio.h>

/* includes for deamonizatio */
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>

#include "filelog.h"
#include "fferror.hpp"
#include "wise_mysql.h"
#include "wise_ipc.h"
#include "wise_rfcomm.hpp"
#include "commonMethods.hpp"
#include "wiseCacheDB.h"
#include "wiseEventMng.h"

using namespace std;

int 	running 			= 0;
uint8_t local_address[5]	= {0x01, 0x02, 0x03, 0x04, 0x05};

int main (void) {
	CacheDB		*cacheDB	= NULL;
	EventMng	*eventMng	= NULL;
	int 		client 		= -1;

    struct stat st = {0};
    if (stat("/tmp/wiseup", &st) == -1) {
        mkdir("/tmp/wiseup", 0777);
        printf("\nCreatng the current working directory...\n");
    }
    
	cacheDB  = new CacheDB ();
	eventMng = new EventMng (cacheDB);
	
	cacheDB->start();
	eventMng->start();

	printf("Starting the listener... [SUCCESS]\n");
	
	while (!running) {
		usleep (10);
    }
	
	cacheDB->stop();
	eventMng->stop();
	
	delete cacheDB;
	delete eventMng;

	exit(EXIT_SUCCESS);
}