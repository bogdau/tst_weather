#include "settings.h"
#include <iostream>

using namespace boost::property_tree;

Settings::Settings(){}

int Settings::refreshTime(){
    std::ifstream jsonFile("../config/config.json");
    if (!jsonFile){
        time = 100;
        std::cerr << "Error opening file\n";
        return time;
    }
    ptree pt;
    json_parser::read_json(jsonFile, pt);
    time = pt.get<float>("data_output_time");
}

int Settings::getTime(){
    return int(time);
}