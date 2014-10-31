/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */

#pragma once

#include <iostream>
#include <pthread.h>
#include <vector>
#include <stdlib.h>

/* includes for wiseup */
#include "wise_ipc.h"
#include "commonMethods.hpp"
#include "wiseCacheDB.h"

#define EVENT_MSG_CACHE_DB_CHANGES		1

using namespace std;

typedef struct {
	uint8_t	msgType;
} event_msg_t;

class EventMng {
public:
	EventMng (CacheDB* cache);
	~EventMng ();
	
	static void apiAlertDBChanges ();
	
	bool start ();
	void stop ();
	
	CacheDB* 			cacheDB;
	bool				m_isWorking;
	sync_context_t		m_lock;
	
private:
	pthread_t	m_worker;
};

