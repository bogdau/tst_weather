#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include <iostream>
#include <QObject>
#include <memory>
#include <QTimer>

#include "include/data_reading.h"
#include "include/settings.h"

class DataManager : public QObject
{
    Q_OBJECT

private:
    std::unique_ptr<DataReading> m_reader;
    QTimer *timer;
    Settings settings;
signals:
    void tempChange(double temp);
    void pressChange(double press);
public:
    DataManager(Settings &set);
    void class_manager(DataReading *dtrd);
    int data_manager();
    void mqtt_publish(double temp_cel, double temp_far, double pressure);
public slots:
    int colect_data();

};

#endif /* __DATA_MANAGER_H__ */

