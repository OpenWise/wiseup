/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <iostream>
#include "wiseNrfAckQueue.h"
#include "commonMethods.hpp"

using namespace std;

void * 
worker (void * args) {
	NrfAckQueue* obj = (NrfAckQueue*)args;
	WiseIPC *ipcPacketsOut = NULL;
	while (obj->m_isWorking) {
		if (!obj->m_isQueueEmpty) {
			nrf24l01_msg_t msg;
			obj->pop(msg);

			ipcPacketsOut = new WiseIPC ("/tmp/wiseup/nrf_outgoing_queue");
			if ( (CommonMethods::getTimestampMillis() - msg.timestamp) > 3000000) {
				cout << "(wiseNrfAckQueue) [ACK worker] Requesting resent, queue size is " << obj->getSize() + 1 << endl;
				
				if (ipcPacketsOut->setClient () == SUCCESS) {
			    	ipcPacketsOut->setBuffer((unsigned char *)&msg);
					msg.isGeneratePacketID = false;
			    	msg.timestamp = CommonMethods::getTimestampMillis();
					if (ipcPacketsOut->sendMsg(sizeof (nrf24l01_msg_t)) == false) { }
				}
			}
			delete ipcPacketsOut;
			
			obj->add(msg);
		}
		
		usleep (10);
	}
}

NrfAckQueue::NrfAckQueue () {
	pthread_cond_init  (&m_lock.cond,  NULL);
    pthread_mutex_init (&m_lock.mutex, NULL);
}

NrfAckQueue::~NrfAckQueue () {
	pthread_mutex_destroy (&m_lock.mutex);
    pthread_cond_destroy  (&m_lock.cond);

    // NOTE: Do we need to clean the vector from all items?
}

bool
NrfAckQueue::start () {
	m_isWorking 	= true;
	m_isQueueEmpty 	= true;
	int error = pthread_create(&m_worker, NULL, worker, this);
    if (error) {
         return false;
    }

    return true;
}

void
NrfAckQueue::stop () {
	m_isWorking = false;
	pthread_cancel (m_worker);
}

void
NrfAckQueue::add (nrf24l01_msg_t &msg) { 
	pthread_mutex_lock (&m_lock.mutex);
	rfcomm_data * packet = NULL;
	for (std::vector<nrf24l01_msg_t>::iterator item = m_messagePull.begin(); item != m_messagePull.end(); ++item) {
		packet = (rfcomm_data *) (item->packet);
		if (!memcmp (msg.packet, packet, 30)) 
			return;
	}
	
	m_messagePull.push_back (msg);
	pthread_mutex_unlock (&m_lock.mutex);
	
	m_isQueueEmpty = false;
}

bool
NrfAckQueue::remove (uint16_t id) {
	pthread_mutex_lock (&m_lock.mutex);
	rfcomm_data * packet = NULL;
	for (std::vector<nrf24l01_msg_t>::iterator item = m_messagePull.begin(); item != m_messagePull.end(); ++item) {
		packet = (rfcomm_data *) (item->packet);
        if (packet->packet_id == id) {
            m_messagePull.erase (item);
            pthread_mutex_unlock (&m_lock.mutex);

            if (m_messagePull.size() == 0) {
				m_isQueueEmpty = true;
			}

            return true;
        }
    }
	pthread_mutex_unlock (&m_lock.mutex);
	
	return false;
}

bool
NrfAckQueue::find (uint16_t id) {
	pthread_mutex_lock (&m_lock.mutex);
	rfcomm_data * packet = NULL;
	for (std::vector<nrf24l01_msg_t>::iterator item = m_messagePull.begin(); item != m_messagePull.end(); ++item) {
		packet = (rfcomm_data *) (item->packet);
        if (packet->packet_id == id) {
            pthread_mutex_unlock (&m_lock.mutex);
            return true;
        }
    }
	pthread_mutex_unlock (&m_lock.mutex);
	
	return false;
}

void
NrfAckQueue::push (nrf24l01_msg_t &msg) {
	add (msg);
}

bool
NrfAckQueue::pop (nrf24l01_msg_t& data) {
	pthread_mutex_lock (&m_lock.mutex);
	nrf24l01_msg_t msg = m_messagePull.back();				
	m_messagePull.pop_back();
	pthread_mutex_unlock (&m_lock.mutex);

	if (m_messagePull.size() == 0) {
		m_isQueueEmpty = true;
	}
	
	memcpy (&data, &msg, sizeof (nrf24l01_msg_t));
	
	return true;
}

int
NrfAckQueue::getSize () {
	return m_messagePull.size();
}