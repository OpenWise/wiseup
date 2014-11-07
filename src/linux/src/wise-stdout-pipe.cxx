/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include <unistd.h>
#include <iostream>
#include <stdio.h>
#include <time.h>

#include "filelog.h"
#include "wise_ipc.h"
#include "fferror.hpp"

using namespace std;

int main (void) {
    unsigned char 		pipe_buff[128];
	unsigned char		msg_buf[512];
    WiseIPC 			*ipcSTDOUT  	= NULL;

    try {
        ipcSTDOUT = new WiseIPC ("/opt/wiseup/stdout_pipe");
        ipcSTDOUT->setServer ();
        ipcSTDOUT->setBuffer (pipe_buff);

        int client = -1;

        time_t today;
        struct tm *now;
        
        while (1) {
            client = ipcSTDOUT->listenIPC ();

			memset (ipcSTDOUT->buff, 0, 128);
            read (client, ipcSTDOUT->buff, 128);
			
			today = time(NULL);
			now = localtime(&today);
			sprintf ((char *) msg_buf, "[%d.%d.%d %d:%d:%d] %s", 
								    now->tm_year + 1900, 
									now->tm_mon + 1, 
									now->tm_mday, 
									now->tm_hour, 
									now->tm_min, 
									now->tm_sec,
								    pipe_buff);
			printf ("# %s\n", msg_buf);
			close (client);
        }
        
        delete ipcSTDOUT;

    } catch (FFError e) {
        std::cout << e.Label.c_str() << std::endl;
        delete ipcSTDOUT;
        return -1;
    }

	return 0;
}

