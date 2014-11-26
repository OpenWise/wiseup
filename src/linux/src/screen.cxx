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