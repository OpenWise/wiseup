/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */
 
#include <iostream>
#include "wiseEventMng.h"

using namespace std;

void * 
eventWorker (void * args) {
	event_msg_t	msg;
	WiseIPC 	*ipcEvent  	= NULL;
	EventMng	*obj = (EventMng*)args;
	
	int client	= -1;
	ipcEvent 	= new WiseIPC ("/tmp/wiseup/event_pipe");
    ipcEvent->setServer ();
    ipcEvent->setBuffer ((unsigned char *)&msg);
	
	while (obj->m_isWorking) {
        client = ipcEvent->listenIPC ();
		memset (ipcEvent->buff, 0, sizeof(event_msg_t));
        read (client, ipcEvent->buff, sizeof(event_msg_t));
		
		switch (msg.msgType) {
			case EVENT_MSG_CACHE_DB_CHANGES: {
				pthread_mutex_lock (&obj->m_lock.mutex);
				for (std::vector<SensorInfo>::iterator item = obj->cacheDB->m_db.begin(); item != obj->cacheDB->m_db.end(); ++item) {
					if (item->sensorInfo.flags.isValueCng == YES) {
						printf ("(EventMng) [eventWorker] EVENT_MSG_CACHE_DB_CHANGES [%d] (%d) -> (%d)\n", 
									item->sensorInfo.sensorType, 
									item->sensorInfo.backup.sensorHWValue, 
									item->sensorInfo.value.sensorHWValue);
						item->sensorInfo.flags.isValueCng = NO;
					}
				}
				pthread_mutex_unlock (&obj->m_lock.mutex);
			}
			break;
		}
		
		close (client);
    }
	
	delete ipcEvent;
}

EventMng::EventMng (CacheDB* cache) {
	cacheDB = cache;
}

EventMng::~EventMng () {
	
}

void
EventMng::apiAlertDBChanges () {
	event_msg_t	msg;
	WiseIPC 	*ipcEvent = new WiseIPC ("/tmp/wiseup/event_pipe");
	if (ipcEvent->setClient () == SUCCESS) {
		ipcEvent->setBuffer((unsigned char *)&msg);
		msg.msgType = EVENT_MSG_CACHE_DB_CHANGES;
		if (ipcEvent->sendMsg(sizeof(event_msg_t)) == false) { }
	} else {
		printf ("(EventMng) [ERROR] - No available event_pipe \n");
	}
	delete ipcEvent;
}

bool
EventMng::start () {
	m_isWorking = true;
	int error 	= pthread_create(&m_worker, NULL, eventWorker, this);
    if (error) {
         return false;
    }

    return true;
}

void
EventMng::stop () {
	m_isWorking = false;
	pthread_cancel (m_worker);
}
