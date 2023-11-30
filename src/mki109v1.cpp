#include "include/mki109v1.h"
#include "driver/lis3mdl_reg.h"
#include <unistd.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <cstdint>
#include <cstring>
#include <QVector>

mki109v1::mki109v1()
{
    if ((i2c_fd = open(i2c_fname.c_str(), O_RDWR)) < 0) {
            char err[200];
            printf(err, "open('%s') in i2c_init", i2c_fname.c_str());;
            perror(err);
        }
    if (ioctl(i2c_fd, I2C_SLAVE, 0x1E) < 0) {
            throw("Not open file /dev/i2c-1, turn on i2c on your device");
            exit(0);
        }
}

int mki109v1::read_bus(int busfd, unsigned char *buf, int bufsize)
{
    return read(busfd, buf, bufsize);
}

int mki109v1::write_bus(int busfd, const unsigned char *buf, int bufsize)
{
    return write(busfd, buf, bufsize);
}

int mki109v1::read_register(int busfd, __uint16_t reg, unsigned char *buf, int bufsize)
{
    unsigned char reg_buf[1];
    int ret;

    reg_buf[0] = (reg >> 0) & 0xFF;

    ret = write_bus(busfd, reg_buf, 1);
    if (ret < 0) {
        printf("Failed to write [0x%02x] (reg: %02x).\n", reg_buf[0], reg);
//        std::cout << errno << std::endl;
        return ret;
    }
    return read_bus(busfd, buf, bufsize);
}

int mki109v1::write_register(int busfd, __uint16_t reg, const unsigned char *buf, int bufsize)
{
    uint8_t *reg_buf = (uint8_t*)malloc(bufsize+1);
    int ret;

    reg_buf[0] = (reg >> 0) & 0xFF;

    memcpy(&reg_buf[1],buf,bufsize);
    ret = write_bus(busfd, reg_buf, bufsize + 1);
    if (ret < 0) {
        printf("Failed to write [0x%02x] (reg: %02x).\n", reg_buf[0], reg);
        std::cout << errno << std::endl;
        return ret;
    }

    free(reg_buf);
    return ret;
}

int32_t mki109v1::write_register_stm (void *mhandle, uint8_t reg , const uint8_t *buf, uint16_t bufsize){
    if( write_register(*((int*)mhandle), reg, buf, bufsize) > 0){
        return 0;
    }
    else{
        return -1;
    }
}


int32_t mki109v1::read_register_stm (void *mhandle, uint8_t reg ,uint8_t *buf, uint16_t bufsize){
    if (read_register(*((int*)mhandle), reg, buf, bufsize)>0){
        return 0;
    }
    else{
        return -1;
    }
}

void  mki109v1::delay_stm (uint32_t millisec){
    usleep(millisec * 1000);
}

QVector<int>  mki109v1::lis3mdl_read_data_polling()
{
    int16_t data_raw_magnetic[3];
    int16_t data_raw_temperature;
    QVector<int> magnetic_mG;
    magnetic_mG.clear();
    magnetic_mG.reserve(3);
//    static float temperature_degC;
    static uint8_t whoamI, rst;

    dev_ctx.write_reg = write_register_stm;
    dev_ctx.read_reg = read_register_stm;
    dev_ctx.mdelay = delay_stm;
    dev_ctx.handle = &i2c_fd;
    /* Initialize platform specific hardware */
    /* Check device ID */
    lis3mdl_device_id_get(&dev_ctx, &whoamI);


    if (whoamI != LIS3MDL_ID)
    throw std::runtime_error("device not recognized"); /*manage here device not found */

    /* Restore default configuration */
     lis3mdl_reset_set(&dev_ctx, PROPERTY_ENABLE);

    do {

    lis3mdl_reset_get(&dev_ctx, &rst);
    } while (rst);

    /* Enable Block Data Update */
    lis3mdl_block_data_update_set(&dev_ctx, PROPERTY_ENABLE);
    //   /* Set Output Data Rate */
    lis3mdl_data_rate_set(&dev_ctx, LIS3MDL_HP_1Hz25);
    //   /* Set full scale */
    lis3mdl_full_scale_set(&dev_ctx, LIS3MDL_16_GAUSS);
    //   /* Enable temperature sensor */
    lis3mdl_temperature_meas_set(&dev_ctx, PROPERTY_ENABLE);
    /* Set device in continuous mode */
    lis3mdl_operating_mode_set(&dev_ctx, LIS3MDL_CONTINUOUS_MODE);

    lis3mdl_md_t type ;
    lis3mdl_operating_mode_get(&dev_ctx, &type);
    /* Read samples in polling mode (no int) */
    uint8_t reg;
    /* Read output only if new value is available */
    lis3mdl_mag_data_ready_get(&dev_ctx, &reg);

    if (reg) {
        memset(data_raw_magnetic, 0x00, 3 * sizeof(int16_t));
        lis3mdl_magnetic_raw_get(&dev_ctx, data_raw_magnetic);
        magnetic_mG.append(1000 * lis3mdl_from_fs16_to_gauss( data_raw_magnetic[0]));
        magnetic_mG.append(1000 * lis3mdl_from_fs16_to_gauss( data_raw_magnetic[1]));
        magnetic_mG.append(1000 * lis3mdl_from_fs16_to_gauss(data_raw_magnetic[2]));
        printf("Magnetic field [mG]:%d X  %d Y  %d Z Gauss\n",
                magnetic_mG[0], magnetic_mG[1], magnetic_mG[2]);
        memset(&data_raw_temperature, 0x00, sizeof(int16_t));
        lis3mdl_temperature_raw_get(&dev_ctx, &data_raw_temperature);
        // temperature_degC = lis3mdl_from_lsb_to_celsius(data_raw_temperature);
        // printf( "Temperature [degC]:%6.2f\n", temperature_degC);
        emit magneticFieldChange(magnetic_mG);
    }
    return magnetic_mG;
}









