/*
 * Author: Yevgeniy Kiveisha <lodmilak@gmail.com>
 * Copyright (c) 2014 OpenWise.
 */
 
#include <iostream>
#include "wiseApplication.h"

using namespace std;

WiseApplicationManager::WiseApplicationManager () {
	pthread_cond_init  (&app_lock.cond, NULL);
	pthread_mutex_init (&app_lock.mutex, NULL);
	
	pthread_cond_init  (&exec_lock.cond, NULL);
	pthread_mutex_init (&exec_lock.mutex, NULL);
}

WiseApplicationManager::~WiseApplicationManager () {
	pthread_mutex_destroy (&app_lock.mutex);
    pthread_cond_destroy  (&app_lock.cond);
	
	pthread_mutex_destroy (&exec_lock.mutex);
    pthread_cond_destroy  (&exec_lock.cond);
	
	delete m_dbconn;
}
	
bool
WiseApplicationManager::getAllApplications () {
	struct DBCredentials cred;
	char 				 query[128];
	
    cred.hostName   = "localhost";
    cred.userId     = "root";
    cred.password   = "sql123";
    cred.database   = "wiseup";
	
	m_dbconn = new MySQL();
	m_dbconn->setupConnection(&cred);
	
	memset (query, 0x0, 64);
	sprintf (query, "call sp_get_applications_info ()");
	
	m_dbconn->executeQuery(query);
	unsigned int fieldsCount = m_dbconn->getFieldsCount ();
	
	MYSQL_ROW row;
	while ((row = mysql_fetch_row(m_dbconn->mySQLResult))) {
		uint64_t applicationID = atoll(row[0]);
		
		app_info_t * existedApp = findApplication (applicationID);
		
		if (existedApp == NULL) {
			// app_id, app_name, app_description, app_install_datetime, app_is_available, database, sensor_id
			app_info_t application;
			application.appId			= applicationID;
			application.appName			= row[1];
			application.appDescription	= row[2];
			application.appInstallDate	= row[3];
			application.isAvailable		= (atoi(row[4]) == 1) ? true : false;
			application.appDatabase		= row[5];
			
			application.attachedSensorIDList.push_back (atoll(row[6]));
			
			pthread_mutex_lock (&app_lock.mutex);
			m_applications.push_back (application);
			pthread_mutex_unlock (&app_lock.mutex);
		} else {
			existedApp->attachedSensorIDList.push_back (atoll(row[6]));
		}
	}
	
	m_dbconn->freeRes();
	
	return true;
}

app_info_t *
WiseApplicationManager::findApplication (uint64_t appID) {
	pthread_mutex_lock (&app_lock.mutex);
    if (m_applications.empty()) {
    } else {
        /* Lets try to find this device in the list */
        for (std::vector<app_info_t>::iterator item = m_applications.begin(); item != m_applications.end(); ++item) {
            if (item->appId == appID) {
				pthread_mutex_unlock (&app_lock.mutex);
                return &(*item);
            }
        }
    }
	
    pthread_mutex_unlock (&app_lock.mutex);
    return NULL;
}

ApllicationExecutor*
WiseApplicationManager::getFreeExecutor () {
	return NULL;
}

void
WiseApplicationManager::printApplicationsInfo () {
	pthread_mutex_lock (&app_lock.mutex);
    if (m_applications.empty()) {
    } else {
        /* Lets try to find this device in the list */
        for (std::vector<app_info_t>::iterator app = m_applications.begin(); app != m_applications.end(); ++app) {
			printf ("%lld, %s, %s, %s, %d, %s\n", app->appId, 
												app->appName.c_str(),
												app->appDescription.c_str(), 
												app->appInstallDate.c_str(),
												app->isAvailable, 
												app->appDatabase.c_str());
			for (std::vector<uint64_t>::iterator sensorID = app->attachedSensorIDList.begin(); sensorID != app->attachedSensorIDList.end(); ++sensorID) {
				printf (" *** %lld\n", *sensorID);
			}
        }
    }
	
    pthread_mutex_unlock (&app_lock.mutex);
}

/* 
 * ******* class ApllicationExecutor ********
 */

void * 
executorWorker (void * args) {
	ApllicationExecutor* obj = (ApllicationExecutor*)args;
}

ApllicationExecutor::ApllicationExecutor () {

}

ApllicationExecutor::~ApllicationExecutor () {

}

void
ApllicationExecutor::execute (char * path) {
	
}
