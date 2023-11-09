#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "boost/property_tree/ptree.hpp"
#include <boost/property_tree/json_parser.hpp>

class Settings{
private:
    int const standart_time = 1000;
    std::string json_path = "config/config.json";
    boost::property_tree::ptree pt;
    int time = 0;
public:
    std::string temperature_unit;
    std::string pressure_unit;

    Settings();
//    Settings(const Settings &) = default;
    void generateJsonFile();
    void loadSettings();
    void saveTempSettings(std::string temp_unit);
    void savePressSettings(std::string press_unit);
    int getSensorPollIntervalMs();
    void loadFromJson(std::string&);
};

#endif /* __SETTINGS_H__ */
