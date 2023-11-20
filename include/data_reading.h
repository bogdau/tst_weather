#ifndef __DATA_READING_H__
#define __DATA_READING_H__
#include "BMP280/BMP280.h"
#include "i2c_impl/RaspberryPi.h"
#include <QObject>


class DataReading : QObject{
public:
    virtual double readTemp() = 0;
    virtual double readPressure() = 0;
};

#endif /* __DATA_READING_H__ */
