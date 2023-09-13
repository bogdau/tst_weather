#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "boost/property_tree/ptree.hpp"
#include <boost/property_tree/json_parser.hpp>

class Settings{
private:
    std::string json_path = "config/config.json";
    int time = 0;
public:
    Settings();
    void generateJsonFile();
    void loadSettings();
    int getSensorPollIntervalMs();
};

#endif /* __SETTINGS_H__ */
