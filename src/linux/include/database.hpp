/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <iostream>

using namespace std;

struct DBCredentials {
    std::string hostName;
    std::string userId;
    std::string password;
    std::string database;
};

class DataBase {
    public:
        DataBase () { };
        virtual ~DataBase () { };
        virtual bool setupConnection (DBCredentials * db) = 0;
        virtual bool executeQuery (string query) = 0;

        DBCredentials databaseInfo;
};
