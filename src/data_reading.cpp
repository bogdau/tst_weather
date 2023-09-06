#include "../include/data_reading.h"

DataReading::DataReading(BMP280 &bmp):
bmp(&bmp)
{
}

void DataReading::dataRefresh(){
    temp_celsium = bmp->getTemperature();
    pressure = bmp->getPressure();
}

double DataReading::getPressure(){
    return pressure;
}

double DataReading::getTempCels(){
    return temp_celsium;
}

double DataReading::getTempFahr(){
    return (temp_celsium * 1.8) + 32;
}
