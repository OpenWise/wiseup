/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <iostream>
#include <pthread.h>
#include <vector>

/* includes for wiseup */
#include "common.h"
#include "wise_rfcomm.hpp"
#include "wise_ipc.h"
#include "commonMethods.hpp"

using namespace std;

class NrfAckQueue {
public:
	NrfAckQueue ();
	~NrfAckQueue ();
	bool start ();
	void stop ();
	void add (nrf24l01_msg_t &msg);
	bool remove (uint16_t id); 
	bool find (uint16_t id);
	void push (nrf24l01_msg_t &msg);
	int getSize ();
	nrf24l01_msg_t & pop ();

	bool					m_isQueueEmpty;
	bool					m_isWorking;
	sync_context_t			m_lock;

private:
	pthread_t       		m_worker;
	vector<nrf24l01_msg_t> 	m_messagePull;
};