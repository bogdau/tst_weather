#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include "boost/property_tree/ptree.hpp"
#include <boost/property_tree/json_parser.hpp>

class Settings{
private:
    int time = 0;
public:
    Settings();
    int loadSettings(std::string json_path);
    int getSensorPollIntervalMs();
};

#endif /* __SETTINGS_H__ */
