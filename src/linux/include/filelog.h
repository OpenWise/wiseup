/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <iostream>

using namespace std;

class FileLog {
    public:
        FileLog (string filename);
        ~FileLog();

        bool writeLog (string data);

    private:
        string  filename;
        FILE*   fdLog;
};
