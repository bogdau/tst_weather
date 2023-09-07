#include <iostream>
#include <cmath>
#include "../include/data_output.h"

DataOutput::DataOutput(){}

void DataOutput::print_info(double temp_cel, double temp_far, double pressure){
    std::printf("Temperature: %.2f ℃, %.2f ℉  Pressure: %.f Pa\n",temp_cel,temp_far,pressure);
}