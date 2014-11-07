/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <iostream>
#include <mysql.h>

#include <database.hpp>

using namespace std;

class MySQL : DataBase {
    public:
        MySQL ();
        ~MySQL ();
        bool setupConnection (DBCredentials * db);
        bool executeQuery (std::string query);
        void freeRes ();

    private:
        MYSQL       *mySQLConnection;
        MYSQL_RES   *mySQLResult;
};
