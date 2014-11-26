/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014, Ingleby Thomas C
 */

#include "spi.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <pthread.h>

#define SPI_MAX_LENGTH 4096
#define MAX_SIZE 64

/**
 * A structure representing the SPI device
 */
struct _spi {
    /*@{*/
    int devfd; /**< File descriptor to SPI Device */
    int mode; /**< Spi mode see spidev.h */
    int clock; /**< clock to run transactions at */
    int lsb; /**< least significant bit mode */
    unsigned int bpw; /**< Bits per word */
    /*@}*/
	pthread_mutex_t mutex;
};

spi_context
spi_init(int bus, int slave)
{
    // mraa_spi_bus_t *spi = mraa_setup_spi(bus);
    if(bus < 0) {
        fprintf(stderr, "Failed. SPI platform Error\n");
        return NULL;
    }
    spi_context dev = (spi_context) malloc(sizeof(struct _spi));
    memset(dev, 0, sizeof(struct _spi));

    char path[MAX_SIZE];
    sprintf(path, "/dev/spidev%u.%u", bus, slave);

    dev->devfd = open(path, O_RDWR);
    if (dev->devfd < 0) {
        fprintf(stderr, "Failed opening SPI Device. bus:%s\n", path);
        free(dev);
        return NULL;
    }
    dev->bpw = 8;
    dev->clock = 4000000;
    dev->lsb = 0;
    dev->mode = 0;
	
	//pthread_mutex_init (&dev->mutex, NULL);

    return dev;
}

result_t
spi_mode(spi_context dev, unsigned short mode)
{
	uint8_t spi_mode = (uint8_t) mode;
	
	if (ioctl (dev->devfd, SPI_IOC_WR_MODE, &spi_mode) < 0) {
		fprintf(stderr, "Failed to set spi mode\n");
        return ERROR_INVALID_RESOURCE;
	}
	
    dev->mode = mode;
    return SUCCESS;
}

result_t
spi_frequency(spi_context dev, int hz)
{
    dev->clock = hz;
    return SUCCESS;
}

result_t
spi_lsbmode(spi_context dev, int lsb)
{
    uint8_t lsb_mode = (uint8_t) lsb;
    if (ioctl (dev->devfd, SPI_IOC_WR_LSB_FIRST, &lsb_mode) < 0) {
        fprintf(stderr, "Failed to set bit order\n");
        return ERROR_INVALID_RESOURCE;
    }
	if (ioctl (dev->devfd, SPI_IOC_RD_LSB_FIRST, &lsb_mode) < 0) {
        fprintf(stderr, "Failed to set bit order\n");
        return ERROR_INVALID_RESOURCE;
    }
	
    dev->lsb = lsb;
    return SUCCESS;
}

result_t
spi_bit_per_word(spi_context dev, unsigned int bits)
{
    dev->bpw = bits;
    return SUCCESS;
}

uint8_t
spi_write(spi_context dev, uint8_t data)
{
	//pthread_mutex_lock (&dev->mutex);
    struct spi_ioc_transfer msg;
    memset(&msg, 0, sizeof(msg));

    uint16_t length = 1;

    uint8_t recv = 0;
    msg.tx_buf = (unsigned long) &data;
    msg.rx_buf = (unsigned long) &recv;
    msg.speed_hz = dev->clock;
    msg.bits_per_word = dev->bpw;
    msg.delay_usecs = 0;
    msg.len = length;
    if (ioctl(dev->devfd, SPI_IOC_MESSAGE(1), &msg) < 0) {
        fprintf(stderr, "Failed to perform dev transfer\n");
		//pthread_mutex_unlock (&dev->mutex);
        return -1;
    }
	
	//pthread_mutex_unlock (&dev->mutex);
    return recv;
}

uint8_t*
spi_write_buf(spi_context dev, uint8_t* data, int length)
{
	//pthread_mutex_lock (&dev->mutex);
    struct spi_ioc_transfer msg;
    memset(&msg, 0, sizeof(msg));

    uint8_t* recv = malloc(sizeof(uint8_t) * length);

    msg.tx_buf = (unsigned long) data;
    msg.rx_buf = (unsigned long) recv;
    msg.speed_hz = dev->clock;
    msg.bits_per_word = dev->bpw;
    msg.delay_usecs = 0;
    msg.len = length;
    if (ioctl(dev->devfd, SPI_IOC_MESSAGE(1), &msg) < 0) {
        fprintf(stderr, "Failed to perform dev transfer\n");
		//pthread_mutex_unlock (&dev->mutex);
		
        return NULL;
    }
	
	//pthread_mutex_unlock (&dev->mutex);
    return recv;
}

result_t
spi_stop(spi_context dev)
{
    close(dev->devfd);
	//pthread_mutex_destroy (&dev->mutex);
    return SUCCESS;
}
