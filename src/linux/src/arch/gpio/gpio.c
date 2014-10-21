/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Copyright (c) 2014, Le Foll Brendan
 */

#include "gpio.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define SYSFS_CLASS_GPIO "/sys/class/gpio"
#define MAX_SIZE 64

gpio_context 
gpio_init(int pin) {
    struct stat dir;
    char        directory[MAX_SIZE];
    int         export = -1;
    char        bu[MAX_SIZE];
    int         length;

    if (pin < 0) {
        return NULL;
    }

    gpio_context ctx = (gpio_context) malloc(sizeof(struct _gpio));
    
    memset(ctx, 0, sizeof(struct _gpio));
    ctx->pin = pin;
    
    snprintf(directory, MAX_SIZE, SYSFS_CLASS_GPIO "/gpio%d/", ctx->pin);
    
    if (stat(directory, &dir) == 0 && S_ISDIR(dir.st_mode)) {
        ctx->owner = 0; // Not Owner
    } else {
        export = open (SYSFS_CLASS_GPIO "/export", O_WRONLY);
        if (export == -1) {
            fprintf (stderr, "Failed to open export for writing!\n");
            return NULL;
        }
        length = snprintf(bu, sizeof(bu), "%d", ctx->pin);
        if (write(export, bu, length*sizeof(char)) == -1) {
            fprintf(stderr, "Failed to write to export\n");
            close(export);
            return NULL;
        }
        ctx->owner = 1;
        close(export);
    }

    return ctx;
}

result_t 
gpio_dir (gpio_context ctx, gpio_dir_t dir) {
    char    filepath[MAX_SIZE];
    char    bu[MAX_SIZE];
    int     direction = -1;
    int     length;

    if (ctx == NULL) {
        return ERROR_INVALID_HANDLE;
    }

    if (ctx->value_fp != -1) {
        close(ctx->value_fp);
        ctx->value_fp = -1;
    }

    snprintf(filepath, MAX_SIZE, SYSFS_CLASS_GPIO "/gpio%d/direction", ctx->pin);
    direction = open(filepath, O_RDWR);

    if (direction == -1) {
        return ERROR_INVALID_RESOURCE;
    }

    switch(dir) {
        case GPIO_OUT:
            length = snprintf(bu, sizeof(bu), "out");
            break;
        case GPIO_IN:
            length = snprintf(bu, sizeof(bu), "in");
            break;
        default:
            close(direction);
            return ERROR_FEATURE_NOT_IMPLEMENTED;
    }

    if (write(direction, bu, length*sizeof(char)) == -1) {
        close(direction);
        return ERROR_INVALID_RESOURCE;
    }

    close(direction);
    return SUCCESS;
}

int 
gpio_read(gpio_context ctx) {
    char bu[2];

    if (ctx == NULL) {
        return ERROR_INVALID_HANDLE;
    }

    if (ctx->value_fp == -1) {
        sprintf(bu, SYSFS_CLASS_GPIO "/gpio%d/value", ctx->pin);
        ctx->value_fp = open(bu, O_RDWR);
        if (ctx->value_fp == -1) {
            return ERROR_INVALID_RESOURCE;
        }
    }

    if (lseek (ctx->value_fp, 0, SEEK_SET) == -1) {
        return ERROR_INVALID_RESOURCE;
    }

    if (read(ctx->value_fp, bu, 2*sizeof(char)) != 2) {
        fprintf(stderr, "Failed to read a sensible value from sysfs");
    }

    lseek(ctx->value_fp, 0, SEEK_SET);

    return strtol(bu, NULL, 10);
}

result_t 
gpio_write (gpio_context ctx, int value) {
    char    bu[MAX_SIZE];
    int     length = 0;

    if (ctx == NULL) {
        return ERROR_INVALID_HANDLE;
    }

    if (ctx->value_fp == -1) {
        sprintf(bu, SYSFS_CLASS_GPIO "/gpio%d/value", ctx->pin);
        ctx->value_fp = open(bu, O_RDWR);
        if (ctx->value_fp == -1) {
            return ERROR_INVALID_RESOURCE;
        }
    }

    if (lseek (ctx->value_fp, 0, SEEK_SET) == -1) {
        return ERROR_INVALID_RESOURCE;
    }

    length = snprintf(bu, sizeof(bu), "%d", value);
    if (write (ctx->value_fp, bu, length*sizeof(char)) == -1) {
        return ERROR_INVALID_HANDLE;
    }

    return SUCCESS;
}

result_t
gpio_close(gpio_context ctx)
{
    int     unexport        = -1;
    int     length          = 0;
    char    bu[MAX_SIZE];

    if (ctx == NULL) {
        return ERROR_INVALID_HANDLE;
    }
    
    if (ctx->value_fp != -1) {
        close(ctx->value_fp);
    }

    if (ctx->owner) {
        unexport = open (SYSFS_CLASS_GPIO "/unexport", O_WRONLY);
        if (unexport == -1) {
            fprintf(stderr, "Failed to open unexport for writing!\n");
            free (ctx);
            return ERROR_INVALID_RESOURCE;
        }

        length = snprintf(bu, sizeof(bu), "%d", ctx->pin);
        if (write (unexport, bu, length*sizeof(char)) == -1) {
            fprintf(stderr, "Failed to write to unexport\n");
            close(unexport);
            free (ctx);
            return ERROR_INVALID_RESOURCE;
        }

        close(unexport);
    }

    free (ctx);
    return SUCCESS;
}
