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

using namespace std;

int 	running 			= 0;
uint8_t local_address[5]	= {0x01, 0x02, 0x03, 0x04, 0x05};
uint8_t wise_stdout_buffer[128];

/* Our process ID and Session ID */
pid_t pid, sid;

void stdout_msg () {
	/* Initiate UNIX socket as stdout */
	WiseIPC *ipcSTDOUT = new WiseIPC ("/tmp/wiseup/stdout_pipe");
	if (ipcSTDOUT->setClient () == SUCCESS) {
		ipcSTDOUT->setBuffer (wise_stdout_buffer);
		if (ipcSTDOUT->sendMsg(128) == false) { }
	}

	delete ipcSTDOUT;
}

void
deamonize () {
    /* Our process ID and Session ID */
    pid_t pid, sid;

    /* Fork off the parent process */
    pid = fork();
    if (pid < 0) {
        exit(EXIT_FAILURE);
    }
    /* If we got a good PID, then
       we can exit the parent process. */
    if (pid > 0) {
        /* Print process id to the screen */
        printf("Process_id of child process %d \n", pid);
        exit (EXIT_SUCCESS);
    }

    /* Change the file mode mask */
    umask(0);
            
    /* Open any logs here */        
            
    /* Create a new SID for the child process */
    sid = setsid();
    if (sid < 0) {
        /* Log the failure */
        exit(EXIT_FAILURE);
    }

    /* Creatng the current working directory */
	struct stat st = {0};
	if (stat("/tmp/wiseup", &st) == -1) {
		mkdir("/tmp/wiseup", 0700);
		printf("\nCreatng the current working directory...\n");
	}

	/* Change the current working directory */
    if ((chdir("/tmp/wiseup")) < 0) {
        /* Log the failure */
		printf("Change the current working directory... [FAILED]\n");
        exit(EXIT_FAILURE);
    }
	printf("\nChange the current working directory... [SUCCESS]\n");
}

int main (void) {
    db_msg_t 			msg;
    WiseIPC 			*ipcDB  		= NULL;
    MySQL   			*dbconn 		= NULL;
	rfcomm_data 		*wisePacket 	= NULL;
	rfcomm_sensor_info 	*sensor_info	= NULL;

    struct stat st = {0};
    if (stat("/tmp/wiseup", &st) == -1) {
        mkdir("/tmp/wiseup", 0777);
        printf("\nCreatng the current working directory...\n");
    }

    /* Daemon-specific initialization goes here */
    // deamonize ();
    
    ipcDB = new WiseIPC ("/tmp/wiseup/db_pipe");
    ipcDB->setServer ();
    ipcDB->setBuffer ((unsigned char *)&msg);

    int client      = -1;

    struct DBCredentials cred;
    cred.hostName   = "localhost";
    cred.userId     = "root";
    cred.password   = "sql123";
    cred.database   = "wiseup";

    char query[512];
    
    dbconn = new MySQL();
    dbconn->setupConnection(&cred);

	printf("Starting the listener... [SUCCESS]\n");

	// printf("Close out the standard file descriptors...\n");
	/* Close out the standard file descriptors */
	// close(STDIN_FILENO);
	// close(STDOUT_FILENO);
	// close(STDERR_FILENO);

	wisePacket = (rfcomm_data *)&msg.packet;
    while (!running) {
        client = ipcDB->listenIPC ();
		memset (ipcDB->buff, 0, sizeof(db_msg_t));
        
        read (client, ipcDB->buff, sizeof(db_msg_t));
		switch (msg.spId) {
			case SP_UPDATE_SENSOR_INFO: {
				uint8_t* data_ptr = wisePacket->data_frame.unframeneted.data;
				sensor_info = (rfcomm_sensor_info *)data_ptr;

				/* Parse the message */
				long long sensor_address = 0;
				long long hub_address = 0;
				int data = 0;
				
				while (wisePacket->data_information.data_size) {
					data_ptr += SENSOR_INFO_DATA_SIZE;
					data = *data_ptr;
					data_ptr += sensor_info->sensor_data_len;
					wisePacket->data_information.data_size = wisePacket->data_information.data_size - 
												(SENSOR_INFO_DATA_SIZE + sensor_info->sensor_data_len);
			
					sensor_address = 0;
					memcpy (&sensor_address, wisePacket->sender, 5);
					sensor_address = (sensor_address << 8) | sensor_info->sensor_address;
					memcpy (&hub_address, wisePacket->sender, 5);

					memset (query, 0x0, 512);
					sprintf (query, "call sp_update_sensor_info ('%lld', '%lld', '%d', '%d', '%d', '%d')", 
								sensor_address, hub_address, sensor_info->sensor_address, sensor_info->sensor_type, 1, data);

					/* Execute MySQL query */
					dbconn->executeQuery(query);
					dbconn->freeRes();

					sensor_info = (rfcomm_sensor_info *)data_ptr;
					
					printf ("%s\n", query);
					/* sprintf ((char *) wise_stdout_buffer, "(wise-dbd) - %s", query);
					stdout_msg (); */
				}
			}
			break;
			case SP_SET_SENSOR_AVAILABILITY: {
				/* Parse the message */
				long long hub_address = 0;
				memcpy (&hub_address, wisePacket->sender, 5);
				
				memset (query, 0x0, 512);
				sprintf (query, "call sp_set_sensor_availability ('%lld', '%d')", hub_address, msg.args[0]);

				/* Execute MySQL query */
				dbconn->executeQuery(query);
				dbconn->freeRes();
				printf ("%s\n", query);
			}
			break;
			case SP_SET_ALL_SENSOR_NOT_CONNECTED:
				memset (query, 0x0, 512);
				sprintf (query, "call sp_set_all_sensor_not_connected ()");
				
				/* Execute MySQL query */
				dbconn->executeQuery(query);
				dbconn->freeRes();
				printf ("%s\n", query);
			break;
		}

		close (client);
    }
    
    delete dbconn;
    delete ipcDB;

	exit(EXIT_SUCCESS);
}

