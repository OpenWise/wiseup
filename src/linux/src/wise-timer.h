/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */
 
#pragma once
 
#include <unistd.h>
#include <iostream>
#include <sys/time.h>
#include <stdio.h>

class WiseTimer {
    public:
        int setTimer (time_t sec) {
            gettimeofday(&m_tv,NULL);
            m_tv.tv_sec += sec;
         
            return 1;
        }
 
        int checkTimer (time_t sec) {
            struct timeval ctv;
            gettimeofday(&ctv,NULL);
         
            if( (ctv.tv_sec > m_tv.tv_sec) )
            {
                gettimeofday(&m_tv,NULL);
                m_tv.tv_sec += sec;
                return 1;
            } else {
                return 0;
            }
        }
    private:
        struct timeval m_tv;
};
