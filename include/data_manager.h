#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include <iostream>
class DataManager
{
public:
    DataManager() = default;
    int data_manager();
    void mqtt_publish(double temp_cel, double temp_far, double pressure);
};

#endif /* __DATA_MANAGER_H__ */

