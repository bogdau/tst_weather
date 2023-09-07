#include "settings.h"
#include <iostream>

using namespace boost::property_tree;

Settings::Settings(){}

int Settings::loadSettings(std::string json_path){
    std::ifstream jsonFile(json_path);
    if (!jsonFile){
        time = 100;
        std::cerr << "Error opening file\n";
        return time;
    }
    ptree pt;
    json_parser::read_json(jsonFile, pt);
    time = pt.get<float>("data_output_time");
}

int Settings::getSensorPollIntervalMs(){
    return int(time);
}