#ifndef __MKI109V1_H__
#define __MKI109V1_H__

#include <iostream>
#include "driver/lis3mdl_reg.h"

class mki109v1
{
private:
    stmdev_ctx_t dev_ctx;
    std::string i2c_fname = "/dev/i2c-1";
    int i2c_fd = -1;

    static int read_bus(int busfd, unsigned char *buf, int bufsize);
    static int write_bus(int busfd, const unsigned char *buf, int bufsize);
    static int read_register(int busfd, __uint16_t reg, unsigned char *buf, int bufsize);
    static int write_register(int busfd, __uint16_t reg, const unsigned char *buf, int bufsize);
    static void delay_stm (uint32_t millisec);
    static int32_t write_register_stm (void *mhandle, uint8_t reg , const uint8_t *buf, uint16_t bufsize);
    static int32_t read_register_stm (void *mhandle, uint8_t reg ,uint8_t *buf, uint16_t bufsize);
public:
    mki109v1();
    void lis3mdl_read_data_polling();
};

#endif // MKI109V1_H
