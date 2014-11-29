/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */
 
#include <iostream>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#include "screen.h"

Screen::Screen (spi_context& spi, uint8_t cs, uint8_t dc, uint8_t rst) : PCD8544 (spi, cs, dc, rst) {

}

Screen::~Screen () {

}

void
Screen::ScreenOne () {
	/*lcd->setTextSize (1);
    lcd->setTextColor(BLACK, WHITE);

    snprintf(Str, sizeof(Str), "TX: %d", lcdCtx.txPacketCount);
    lcd->setCursor(1, 1);
    lcd->print(Str);

    snprintf(Str, sizeof(Str), "RX: %d", lcdCtx.rxPacketCount);
    lcd->setCursor(1, 10);
    lcd->print(Str);

    lcdCtx.cpuTemperature = getCPUTemp ();
    snprintf(Str, sizeof(Str), "CPU (C): %dc", lcdCtx.cpuTemperature);
    lcd->setCursor(1, 20);
    lcd->print(Str);

    lcdCtx.cpuUsage = getCPULoad ();
    snprintf(Str, sizeof(Str), "CPU (%): %d%", lcdCtx.cpuUsage);
    lcd->setCursor(1, 30);
    lcd->print(Str);

    lcd->refresh ();*/
}