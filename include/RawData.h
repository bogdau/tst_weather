#ifndef RAWDATA_H
#define RAWDATA_H

#include "data_reading.h"

class RawData : public DataReading{
public:
    double readTemp() override;
    double readPressure() override;
};


#endif // RAWDATA_H
