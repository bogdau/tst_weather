#ifndef BMP280DATA_H
#define BMP280DATA_H
#include <iostream>
#include <memory>
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <iostream>
#include <iomanip>
#include <string.h>
#include "data_reading.h"

class BMP280Data : public DataReading{
private:
//    std::unique_ptr<RPI_I2CInputOutput> RPI_IO;
//    std::unique_ptr<BMP280> bmp280;
    RPI_I2CInputOutput *RPI_IO;
    BMP280 *bmp280;

    double temperature = 0.0;
    double pressure = 0.0;

public:
    BMP280Data();
    int init();
    double readTemp() override;
    double readPressure() override;
    void signal_callback(int);
    void initializeSignalCallback();
    void initializing();
    void calibration();

};
#endif // BMP280DATA_H
