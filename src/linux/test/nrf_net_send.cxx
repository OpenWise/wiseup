#include <string.h>
#include <unistd.h>
#include <iostream>
#include "nrf24l01.h"
#include <signal.h>
#include <stdio.h>

uint8_t local_address[5]     = {0x05, 0x04, 0x03, 0x02, 0x01};
uint8_t dest_address[5]      = {0x01, 0x01, 0x01, 0x01, 0x01};

int running = 0;
comm::NRF24L01      *sensor         = NULL;
comm::WiseRFComm    *net            = NULL;

void
sig_handler(int signo)
{
    printf("got signal\n");
    if (signo == SIGINT) {
        printf("exiting application\n");
        running = 1;
    }
}

void
dataHandling () {
}

void
netLayerDataArrivedHandler () {
    dataHandling ();
}

void
netLayerBroadcastArrivedHandler () {
    dataHandling ();
}

void
nrfRecieveHandler () {
    std::cout << "Transmitter :: recieve data ....";
    net->parseRXRawData ();
}

int
main(int argc, char **argv)
{
    uint32_t dummyData = 333333;

    printf("Initiating nrf24l01...\n");
    /* Init nRF24l01 communication */
    sensor = new comm::NRF24L01 (17, 22);
    net = new comm::WiseRFComm (sensor, nrfRecieveHandler);
    net->setSender (local_address);
    net->setDataHandler (netLayerDataArrivedHandler);
    net->setBroadcastHandler (netLayerBroadcastArrivedHandler);

    signal(SIGINT, sig_handler);

    while (!running) {
        net->sendPacket ((uint8_t *) dest_address);
        std::cout << "Transmitter :: sending data ...." << dummyData << std::endl;
        usleep (3000000);
        dummyData += 3000;
    }

    std::cout << "exiting application" << std::endl;

    delete sensor;
    return 0;
}
