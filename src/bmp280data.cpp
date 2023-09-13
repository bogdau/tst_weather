#include "include/bmp280data.h"
#include <iostream>
#include <memory>

int BMP280Data::init(){
    bmp = std::make_unique<BMP280>();
    uint8_t ret = bmp->init(BMP280::CSBState::High, BMP280_FILTER_COEFF_16, BMP280_OS_16X, BMP280_OS_2X, BMP280_ODR_62_5_MS);
    if(bmp->read() != 0){
            std::cerr << "can't read data from sensor" << std::endl;
            bmp->delay_ms(120);
    }

    if (ret != 0){
            return true;
    }
    else{
        return false;
    }
}

double BMP280Data::readTemp(){
    return bmp->getTemperature();
}

double BMP280Data::readPressure(){
    return bmp->getPressure();
}
