/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "wise_mysql.h"
#include "filelog.h"
#include "fferror.hpp"

using namespace std;

MySQL::MySQL () {

}

MySQL::~MySQL () {
    mysql_close (this->mySQLConnection);
}

bool 
MySQL::setupConnection (DBCredentials * db) {
    MYSQL *MySQLConRet;
    this->mySQLConnection = mysql_init(NULL);

    try {
        MySQLConRet = mysql_real_connect( this->mySQLConnection,
                                          db->hostName.c_str(), 
                                          db->userId.c_str(), 
                                          db->password.c_str(), 
                                          db->database.c_str(), 
                                          0, 
                                          NULL,
                                          0 );

        if (MySQLConRet == NULL) {
            throw FFError( (char*) mysql_error(this->mySQLConnection) );
        }
    }
    catch (FFError e) {
        cout << e.Label.c_str() << endl;
        return false;
    }

    return true;
}

bool 
MySQL::executeQuery (std::string query) {
    int mysqlStatus = 0;

    try {
        mysqlStatus = mysql_query (this->mySQLConnection, query.c_str());

        if (mysqlStatus) {
            throw FFError( (char*)mysql_error(this->mySQLConnection) );
        } else {
            this->mySQLResult = mysql_store_result (this->mySQLConnection); // Get the Result Set
            
        }
    }
    catch ( FFError e ) {
        cout << e.Label.c_str() << endl;
        mysql_close (this->mySQLConnection);
        return false;
    }

    return true;
}

void 
MySQL::freeRes () {
    mysql_free_result (this->mySQLResult);
}
