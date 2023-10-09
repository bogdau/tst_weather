#include <iostream>
#include <iomanip>
#include <cmath>
#include "../include/data_output.h"

DataOutput::DataOutput(){}

void DataOutput::print_info(double temp_cel, double temp_far, double pressure){
    std::cout <<"Temperature: " <<std::setfill('0') << std::setw(5) << std::fixed << std::setprecision(2) << temp_cel << "°C "<<  temp_far <<  "°F Pressure: " << pressure << std::endl;
}
