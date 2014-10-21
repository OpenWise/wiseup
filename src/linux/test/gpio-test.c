/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014 Wiseup.
 */

#include "gpio.h"
#include <unistd.h>

int main (int argc, char ** argv) {
    int i = 0;
    int value = 0;

    gpio_context gpio_test = gpio_init (22);
    gpio_dir (gpio_test, GPIO_OUT);

    for (i = 0; i < 10; i++) {
        gpio_write (gpio_test, value);
        value = !value;
        usleep(1000000);
    }

    gpio_write (gpio_test, 0x0);
    gpio_close (gpio_test);

    return 0;
}
