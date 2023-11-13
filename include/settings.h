#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "boost/property_tree/ptree.hpp"
#include <boost/property_tree/json_parser.hpp>
#include "include/aws.h"
#include <sqlite3.h>

class Settings
{
private:
    std::string const dir = "config/";
    std::string const config_file = "config/settings.db";
    sqlite3 *db;
    int time = 0;

public:
    std::string temperature_unit;
    std::string pressure_unit;

    Settings();
    ~Settings();
    void loadSettings();
    void saveTempSettings(std::string temp_unit);
    void savePressSettings(std::string press_unit);
    void saveTimeSettings(int);
    int getSensorPollIntervalMs();
};

#endif /* __SETTINGS_H__ */
