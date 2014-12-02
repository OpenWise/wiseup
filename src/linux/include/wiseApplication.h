/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */

#pragma once

#include <iostream>
#include <pthread.h>
#include <vector>
#include <stdlib.h>

#include "wise_rfcomm.hpp"
#include "wise_ipc.h"
#include "commonMethods.hpp"
#include "wise_mysql.h"

using namespace std;

typedef struct {
	uint64_t 	appId;
	std::string appName;
	std::string	appDescription;
	std::string	appInstallDate;
	bool		isAvailable;
	std::string	appDatabase;
	vector<uint64_t> attachedSensorIDList;
} app_info_t;

class WiseApplicationManager {
public:
	WiseApplicationManager ();
	~WiseApplicationManager ();
	
	bool getAllApplications ();
	app_info_t * findApplication (uint64_t appID);
	
	sync_context_t	lock;
	
private:
	vector<app_info_t>	m_applications;
	MySQL *				m_dbconn;
};
