#ifndef BMP280DATA_H
#define BMP280DATA_H
#include <iostream>
#include <memory>
#include "data_reading.h"

class BMP280Data : public DataReading{
private:
    std::unique_ptr<BMP280>bmp;

public:
    int init();
    double readTemp() override;
    double readPressure() override;
};
#endif // BMP280DATA_H
