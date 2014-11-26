/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Intel Corporation.
 */

#include "pcd8544.h"
 
#pragma once

class Screen : public PCD8544 {
public:
	Screen (spi_context& spi, uint8_t cs, uint8_t dc, uint8_t rst);
	~Screen ();
};