#ifndef __DATA_OUTPUT_H__
#define __DATA_OUTPUT_H__

#include "data_reading.h"
class DataOutput
{
public:
    DataOutput();
    void print_info(double temp_cel, double temp_far, double pressure);
};

#endif /* __DATA_OUTPUT_H__ */

