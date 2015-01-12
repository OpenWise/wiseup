/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <errno.h>
#include <unistd.h>
#include <iostream>
#include <vector>
#include <signal.h>
#include <stdio.h>
#include <pthread.h>
#include <sys/time.h>
#include <string>
#include "json/json.h"
#include <stdlib.h>     /* srand, rand */
#include <cstdlib>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

/* includes for wiseup */
#include "common.h"
#include "nrf24l01.h"
#include "filelog.h"
#include "fferror.hpp"
#include "wise_ipc.h"
#include "wise_rfcomm.hpp"
#include "wise-timer.h"
#include "wise_client_handler.h"
#include "commonMethods.hpp"
#include "screen.h"
#include "hiredis.h"
#include "async.h"
#include "adapters/libevent.h"

using namespace std;

#define DEAMON_REQUEST_IS_ALIVE 1

uint8_t local_address[5]     = {0x01, 0x02, 0x03, 0x04, 0x05};

int 			    	running 	        = 0;
comm::NRF24L01*			sensor 	       		= NULL;
comm::WiseRFComm*		net                	= NULL;
WiseClientHandler*		clientHandler      	= NULL;
WiseCommandHandler*		cmdHandler         	= NULL;
// nrfActionTaskMng*   	wiseNRFActionTask   = NULL;
screen_context      	deviceScreenCtx;
redisContext*			redisCtx			= NULL;
pthread_t       		ipcNRFOutListenerThread;
pthread_t       		redisSubscriberThread;

void
dataHandling (rfcomm_data * packet) {
    wise_status_t deviceStatus = clientHandler->registrationCheck (packet); // Chick the registration ID.
    if (deviceStatus == DISCOVERY) {
		clientHandler->addNewClient (packet->sender); // Add new client to local DB
		clientHandler->sendRegistration (packet); // Send gateway address back to the device
	} else if (deviceStatus == CONNECTED) {
		clientHandler->updateSensorInfo (packet); // Update the sensors info
        cmdHandler->commandHandler (packet); // Execute the requested command
		printf ("(wise-nrfd) [dataHandling] UPDATE INFO \n");
    } else if (deviceStatus == UNKNOWN) { }

    deviceScreenCtx.rxPacketCount++;
}

/*
 * 3d layer - the nRF24l01 is the 2d layer witch provide the data from
 * the air. This layer get the data and remove the ecryption, check if 
 * the data relevant to this device and more.
 * 
 * This method is a handler to the relevant data to this device.
 */
void
netLayerDataArrivedHandler (void * args) {
    dataHandling ((rfcomm_data *) args);
}

/*
 * 3d layer - the nRF24l01 is the 2d layer witch provide the data from
 * the air. This layer get the data and remove the ecryption, check if 
 * the data relevant to this device and more.
 * 
 * This method handling broadcast data.
 */
void
netLayerBroadcastArrivedHandler (void * args) {
    dataHandling ((rfcomm_data *) args);
}

void *
outgoingMessageListener (void *) {
    /* UNIX domain socket listener for nrf24l01 outgoing packets */
    WiseIPC *ipcNrfOut = NULL;
    try {
        ipcNrfOut = new WiseIPC ("/tmp/wiseup/nrf_outgoing_queue");
        ipcNrfOut->setServer ();
		
		int client = -1;
		nrf24l01_msg_t msg;
        
        while (true) {
            client = ipcNrfOut->listenIPC ();
			ipcNrfOut->setBuffer ((uint8_t *)&msg);
            ipcNrfOut->readMsg (client, sizeof (nrf24l01_msg_t));
			
			msg.timestamp = CommonMethods::getTimestampMillis(); // Add time-stamp to the message
			srand (time(NULL)); // Add random id to the packet
			msg.packet.packet_id = rand() % 65500 + 1;
			
			memcpy (net->ptrTX, &msg.packet, MAX_BUFFER);
			net->sendPacket (msg.packet.target); // Send the packet
			
			deviceScreenCtx.txPacketCount++; // Increase TX packet count
            
            // Print to the command line
			printf ("(wise-nrfd) [outgoingNrf24l01] Sending to [%d %d %d %d %d] from [%d %d %d %d %d]\n", 
                                                msg.packet.target[0], msg.packet.target[1], 
                                                msg.packet.target[2], msg.packet.target[3], 
                                                msg.packet.target[4], 
                                                msg.packet.sender[0], msg.packet.sender[1], 
                                                msg.packet.sender[2], msg.packet.sender[3], 
                                                msg.packet.sender[4]);
												
            close (client);
        }
    } catch (FFError e) {
        std::cout << e.Label.c_str() << std::endl;
    }

    delete ipcNrfOut;
    return NULL;
}

void subCallback(redisAsyncContext *c, void *r, void *priv) {
    redisReply * reply = (redisReply *)r;
    if (reply == NULL) return;
    if ( reply->type == REDIS_REPLY_ARRAY && reply->elements == 3 ) {
        if ( strcmp( reply->element[0]->str, "subscribe" ) != 0 ) {
            printf( "Received[%s] channel %s: %s\n", (char*)priv, reply->element[1]->str, reply->element[2]->str );
			
			Json::Value root;
			Json::Reader reader;
			bool parsingSuccessful = reader.parse( reply->element[2]->str, root );
			if (!parsingSuccessful) {
				std::cout  << "Failed to parse configuration\n"
						   << reader.getFormattedErrorMessages();
			} else {
				long long   sensorAddress	= root.get("id", 0).asInt64();
				int         sensorAction	= root.get("action", 0).asInt();
				
				std::cout  	<< "Sensor action request:\n"
							<< "Address = " << sensorAddress << "\n"
							<< "Action  = " << sensorAction << "\n";
				clientHandler->sendSensorCommand (sensorAddress, sensorAction);
			}
        }
    }
}

void connectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        return;
    }
    printf("Connected...\n");
}

void disconnectCallback(const redisAsyncContext *c, int status) {
    if (status != REDIS_OK) {
        return;
    }
    printf("Disconnected...\n");
}

void *
redisSubscriber (void *) {
	signal(SIGPIPE, SIG_IGN);
    struct event_base *base = event_base_new();

    redisAsyncContext *redisAsyncCtx = redisAsyncConnect ("127.0.0.1", 6379);
    if (redisAsyncCtx->err) {
        return NULL;
    }

    redisLibeventAttach (redisAsyncCtx, base);
    redisAsyncSetConnectCallback (redisAsyncCtx, connectCallback);
    redisAsyncSetDisconnectCallback (redisAsyncCtx, disconnectCallback);
    redisAsyncCommand (redisAsyncCtx, subCallback, (char*) "sub", "SUBSCRIBE SENSOR-ACTION");

    event_base_dispatch (base);
}

int
main (int argc, char **argv)
{
	redisCtx = redisConnect("127.0.0.1", 6379);
	if (redisCtx->err) {
		exit (EXIT_FAILURE);
	}
	
	clientHandler = new WiseClientHandler (redisCtx);
	
    umask(0);
    struct stat st = {0};
    if (stat ("/tmp/wiseup", &st) == -1) {
        mkdir ("/tmp/wiseup", 0777);
    }

    int error = pthread_create (&ipcNRFOutListenerThread, NULL, outgoingMessageListener, NULL);
    if (error) {
        exit(EXIT_FAILURE);
    }
	
	error = pthread_create (&redisSubscriberThread, NULL, redisSubscriber, NULL);
    if (error) {
        exit(EXIT_FAILURE);
    }
		
	printf("Initiating nrf24l01...\n");
    sensor 	= new comm::NRF24L01 (17, 22); // Initiating nRF24l01 layer (hardware)
    net 	= new comm::WiseRFComm (sensor, netLayerDataArrivedHandler, netLayerBroadcastArrivedHandler); // Initiating network layer
    net->setSender (local_address);
    printf("Starting the listener... [SUCCESS]\n");
	
	while (!running) {
        net->listenForIncoming ();
	}
    
	pthread_exit (NULL);
	redisFree(redisCtx);
    exit (EXIT_SUCCESS);
}
