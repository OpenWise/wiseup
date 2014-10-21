/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "filelog.h"
#include "fferror.hpp"

using namespace std;

FileLog::FileLog (string filename) {
    this->fdLog     = NULL;
    this->filename  = filename;

    umask(0);

    /* Change the current working directory */
    if ((chdir("/home/pi/Development")) < 0) {
        /* Log the failure */
        throw FFError((char *)"Cannot change directory");
    }

    printf ("%s\n", filename.c_str());
    this->fdLog = fopen (filename.c_str(), "w+");
    
    if (this->fdLog == NULL) {
        printf ("ERROR FDLOG\n");
    }
}

FileLog::~FileLog () {
    if (this->fdLog != NULL) {
        fclose(this->fdLog);
    }
}

bool
FileLog::writeLog(string data) {
    if (this->fdLog != NULL) {
        fprintf(this->fdLog, "%s", data.c_str());
        fflush(this->fdLog);
    } else {
        printf ("ERROR \n");
    }

    return true;
}
