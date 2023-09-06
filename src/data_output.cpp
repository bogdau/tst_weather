#include <iostream>
#include "../include/data_output.h"

DataOutput::DataOutput(/* args */)
{
}

int DataOutput::print_info(){
    BMP280 bmp;
    
    uint8_t ret = bmp.init(BMP280::CSBState::High, BMP280_FILTER_COEFF_16, BMP280_OS_16X, BMP280_OS_2X, BMP280_ODR_62_5_MS);
    if (ret != 0)
    {
        return -1;
    }

    int i = 0;
    DataReading dtrd(bmp);
    while (bmp.read() == 0)
    {
        double pressure = bmp.getPressure();
        double temp = bmp.getTemperature();
        
        printf("%d: P = %.1f Pa\tT = %.1f °C\tAlt = %.2f m\n\r", i, pressure, temp);
       

        i++;
        bmp.delay_ms(100);
    }
    return 0;
}