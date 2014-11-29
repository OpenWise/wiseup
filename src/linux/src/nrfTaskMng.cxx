/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */
 
#include <iostream>
#include "nrfTaskMng.h"

using namespace std;

nrfActionTaskMng::nrfActionTaskMng (uint64_t interval) {
	m_interval = interval;

	pthread_cond_init  (&m_lock.cond, NULL);
	pthread_mutex_init (&m_lock.mutex, NULL);
}

nrfActionTaskMng::~nrfActionTaskMng () {
	pthread_mutex_destroy (&m_lock.mutex);
    pthread_cond_destroy  (&m_lock.cond);
}

void * 
nrfActionTaskExecuterWorker (void * args) {
	nrfActionTaskMng* obj 	= (nrfActionTaskMng*)args;
	while (obj->m_isWorking) {
		pthread_mutex_lock (&obj->m_lock.mutex);
		if (obj->m_tasks.size() > 0) {
			for (std::vector<nrf_action_task>::iterator item = obj->m_tasks.begin(); item != obj->m_tasks.end(); ++item) {
				if (CommonMethods::getTimestampMillis() - item->timestamp > obj->m_interval) {
					if (item->sensor != NULL) {
						if (item->sensor->value.sensorHWValue != item->sensor->value.sensorUIValue) {
							/* Send to OUTGOING queue */
						    WiseIPC *ipcPacketsOut = new WiseIPC ("/tmp/wiseup/nrf_outgoing_queue");
						    if (ipcPacketsOut->setClient () == SUCCESS) {
						        ipcPacketsOut->setBuffer((unsigned char *)&item->packet);
						        if (ipcPacketsOut->sendMsg(32) == false) { }
						        else {
						        	printf ("(nrfActionTaskMng) [nrfActionTaskExecuterWorker] RESENDING TO [%d %d %d %d %d]\n", 
						                                                item->packet.target[0], item->packet.target[1], 
						                                                item->packet.target[2], item->packet.target[3], 
						                                                item->packet.target[4]);
						        }
						    }

						    delete ipcPacketsOut;
						} else {
							printf ("(nrfActionTaskMng) [nrfActionTaskExecuterWorker] REMOVING FOR [%d %d %d %d %d]\n", 
						                                                item->packet.target[0], item->packet.target[1], 
						                                                item->packet.target[2], item->packet.target[3], 
						                                                item->packet.target[4]);
							obj->m_tasks.erase (item);
						}
					}
				}
			}
		}
		pthread_mutex_unlock (&obj->m_lock.mutex);

		usleep (obj->m_interval);
	}
}

 bool
nrfActionTaskMng::start () {
	m_isWorking = true;

    int error 	= pthread_create(&m_workerTaskExecuter, NULL, nrfActionTaskExecuterWorker, this);
    if (error) {
        return false;
    }

    return true;
}

void
nrfActionTaskMng::stop () {
	m_isWorking = false;
	pthread_cancel (m_workerTaskExecuter);
}

void
nrfActionTaskMng::apiAddTask (sensor_info_t* sensor, rfcomm_data* packet) {
	nrf_action_task task;
	task.sensor = sensor;
	task.timestamp = CommonMethods::getTimestampMillis();
	memcpy (&task.packet, packet, sizeof (rfcomm_data));

	printf ("(nrfActionTaskMng) [apiAddTask] ADDING TO RESEND (%lld)\n", sensor->sensorAddress);

	pthread_mutex_lock (&m_lock.mutex);
	m_tasks.push_back (task);
	pthread_mutex_unlock (&m_lock.mutex);
}

void
nrfActionTaskMng::apiRemoveTask (long long sensorAddress) {
	pthread_mutex_lock (&m_lock.mutex);
	for (std::vector<nrf_action_task>::iterator item = m_tasks.begin(); item != m_tasks.end(); ++item) {
		if (item->sensor->sensorAddress == sensorAddress) {
			m_tasks.erase (item);
			pthread_mutex_unlock (&m_lock.mutex);
			return;
		}
	}
	pthread_mutex_unlock (&m_lock.mutex);
}
