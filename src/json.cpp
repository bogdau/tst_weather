#include "json.h"
#include <iostream>

using namespace boost::property_tree;

Json::Json(){
    std::ifstream jsonFile("../config/config.json");
    if (!jsonFile){
        std::cerr << "Error opening file\n";
    }
    ptree pt;
    json_parser::read_json(jsonFile, pt);
    time = pt.get<int>("data_output_time");
}

int Json::getTime(){
    return time;
}