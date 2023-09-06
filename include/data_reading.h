#ifndef __DATA_READING_H__
#define __DATA_READING_H__
#include "../include/raspberry_bmp280.h"

class DataReading{
private:
    double pressure = 0;
    double temp_celsium = 0;
    double temp_farenheit = 0;

    BMP280 *bmp = nullptr;

public:
    DataReading(BMP280 &bmp);
    
    void dataRefresh();
    double getPressure();
    double getTempCels();
    double getTempFahr();
};

#endif /* __DATA_READING_H__ */