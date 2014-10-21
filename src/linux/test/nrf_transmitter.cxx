/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <string.h>
#include <unistd.h>
#include <iostream>
#include "nrf24l01.h"
#include <signal.h>
#include <stdio.h>

uint8_t local_address[5]     = {0x05, 0x04, 0x03, 0x02, 0x01};
uint8_t broadcast_address[5] = {0x01, 0x01, 0x01, 0x01, 0x01};

int running = 0;
comm::NRF24L01 *sensor = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        running = 1;
    }
}

void nrf_handler () {
}

int
main(int argc, char **argv)
{
    uint32_t dummyData = 333333;
    sensor = new comm::NRF24L01(17, 22);
    sensor->setSourceAddress ((uint8_t *) local_address);
    sensor->setDestinationAddress ((uint8_t *) broadcast_address);
    sensor->setPayload (MAX_BUFFER);
    sensor->configure ();
    sensor->setSpeedRate (comm::NRF_250KBPS);
    sensor->setChannel (99);
    sensor->dataRecievedHandler = nrf_handler;

    signal(SIGINT, sig_handler);

    while (!running) {
        memcpy (sensor->m_txBuffer, &dummyData, sizeof (dummyData));
        sensor->setDestinationAddress ((uint8_t *) broadcast_address);
        sensor->send ();
        std::cout << "Transmitter :: sending data ...." << dummyData << std::endl;
        usleep (3000000);
        dummyData += 3000;
    }

    std::cout << "exiting application" << std::endl;

    delete sensor;
    return 0;
}
