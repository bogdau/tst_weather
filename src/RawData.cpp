#include "include/RawData.h"
#include <random>

double RawData::readTemp(){
    return 0 + rand() % 30;
}

double RawData::readPressure(){
    return 90000 + rand() % 11000;
}
