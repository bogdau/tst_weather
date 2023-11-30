#ifndef __DATA_MANAGER_H__
#define __DATA_MANAGER_H__

#include <iostream>
#include <QObject>
#include <memory>
#include <QTimer>
#include <map>

#include "include/data_reading.h"
#include "include/settings.h"
#include "include/mki109v1.h"
#include "data_base.h"
#include "aws.h"
#include "dispay_sh1106/oled.h"

class DataManager : public QObject
{
    Q_OBJECT

private:
    enum Command{
        SET_TEMPERATURE_UNIT,
        SET_PRESSURE_UNIT,
        SET_TIME_INTERVAL,
        DATABASE
    };
    std::map<std::string, Command> string_to_Comand = {
        {"temperature", Command::SET_TEMPERATURE_UNIT},
        {"pressure", Command::SET_PRESSURE_UNIT},
        {"time", Command::SET_TIME_INTERVAL},
        {"database", Command::DATABASE}
    };
    std::unique_ptr<DataReading> m_reader;
    QTimer *timer;
    Settings settings;
    mki109v1 *mki;
    data_base db_mag;
    data_base db_temp;
    oled oled_display;
signals:
    void tempChange(double temp);
    void pressChange(double press);

public:
    DataManager(Settings &set, mki109v1 &mki);
    void class_manager(DataReading *dtrd);
    int data_manager();
    void mqtt_publish(double temp_cel, double temp_far, double pressure);
    void command_selector(std::string &, aws &);
public slots:
    void colect_data();
};

#endif /* __DATA_MANAGER_H__ */
