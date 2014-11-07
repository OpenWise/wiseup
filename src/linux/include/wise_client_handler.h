/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#pragma once

#include <cstdlib>
#include <cstring>
#include <stdlib.h>

#include <iostream>
#include <algorithm>
#include <vector>
#include <iterator>
#include <stdio.h>

#include "nrf24l01.h"
#include "wiseDBMng.h"

using namespace std;

typedef enum {
    DISCOVERY       =   0,
    CONNECTED       =   1,
    UNKNOWN         =   99
} wise_status_t;

class WiseClient {
    public:        
        WiseClient (uint8_t * addr) {
            memcpy (address, addr, 5);
        }

        friend bool operator== ( const WiseClient &wc1, const WiseClient &wc2 ) {
            for (int i = 0; i < 5; i++) {
                if (wc1.address[i] != wc2.address[i])
                    return false;
            }
            return true;
        }
        
        void printAddress () {
            printf ("Client address ( ");
            for (int i = 0; i < 5; i++) {
                printf ("%x ", address[i]);
            } printf (")\n");
        }
        
        uint8_t         address[5];
        uint64_t        timestamp;
        wise_status_t   status;
};

class WiseClientHandler {
    public:
        WiseClientHandler (comm::WiseRFComm * net);
        ~WiseClientHandler ();

        /*
         * Need to update this method with stl find method.
         */
        wise_status_t   registrationCheck (rfcomm_data* wisePacket);
        void            removeUnusedDeveices ();
        WiseClient*     findClient (uint8_t * address);
        void            sendRegistration ();

    private:
        /* List of WiseUp clients */
        vector<WiseClient>  m_clients;
        comm::WiseRFComm*   m_net;
};

class WiseCommandHandler {
    public:
        WiseCommandHandler (comm::WiseRFComm * net);
        ~WiseCommandHandler ();

        void    commandHandler (rfcomm_data* wisePacket);

    private:
        comm::WiseRFComm*   m_net;
};
