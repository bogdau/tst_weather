#include "include/settings.h"
#include <iostream>

using namespace boost::property_tree;

Settings::Settings(){
    std::ifstream jsonFile(json_path);
    if (!jsonFile){
        time = 100;
        std::cerr << "Error opening file, generate another one\n";
        generateJsonFile();
    }
}

void Settings::generateJsonFile(){
    boost::property_tree::ptree pt;

    pt.put("data_output_time", "1000");

    boost::property_tree::write_json(json_path, pt);
}

void Settings::loadSettings(){
    std::ifstream jsonFile(json_path);
    ptree pt;
    json_parser::read_json(jsonFile, pt);
    time = pt.get<int>("data_output_time");
}

int Settings::getSensorPollIntervalMs(){
    return int(time);
}
