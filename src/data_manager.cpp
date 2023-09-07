#include <iostream>
#include "data_manager.h"
#include "data_output.h"
#include "data_reading.h"

int DataManager::data_manager(){ 
    BMP280 bmp;
    
    uint8_t ret = bmp.init(BMP280::CSBState::High, BMP280_FILTER_COEFF_16, BMP280_OS_16X, BMP280_OS_2X, BMP280_ODR_62_5_MS);
    if (ret != 0)
    {
        return -1;
    }

    DataReading dtrd(bmp);
    DataOutput dtot;


    while(true){
        if(bmp.read() != 0){
            std::cerr << "can't read data" << std::endl;
            bmp.delay_ms(120);
        }
        dtrd.dataRefresh();
        dtot.print_info(dtrd.getTempCels(),dtrd.getTempFahr(),dtrd.getPressure() );

        bmp.delay_ms(120);
    }

    
    return 0;
}