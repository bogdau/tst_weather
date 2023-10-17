#include "include/settings.h"
#include <iostream>
#include <filesystem>
#include <stdio.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

using namespace boost::property_tree;

Settings::Settings(){
    std::ifstream jsonFile(json_path);
    if (!jsonFile){
        time = standart_time;
        std::cerr << "Error opening file, generate another one\n";
        generateJsonFile();
    }
}

void Settings::generateJsonFile(){
    mkdir("config",0777);
    std::remove(json_path.c_str());

    pt.put("data_output_time", standart_time);
    pt.put("temperature", "°C");
    pt.put("pressure", "Pa");

    boost::property_tree::write_json(json_path, pt);
}

void Settings::loadSettings(){
    std::ifstream jsonFile(json_path);
    json_parser::read_json(jsonFile, pt);

    time = pt.get<int>("data_output_time");
    temperature_unit = pt.get<std::string>("temperature");
    pressure_unit = pt.get<std::string>("pressure");
    int fd = open("/tmp/fifo", O_RDONLY);
    char str1[100];
    int rfd;
    if ((rfd = open("/tmp/fifo", O_RDONLY|O_NONBLOCK)) < 0){
          perror("open() error for read end");
        if(read(fd,str1,50) != -1){
            std::string file_data(str1,50);
            std::cout << file_data << std::endl;
        }
    }
}

void Settings::saveTempSettings(std::string temp_unit){
    loadSettings();
    pt.put("data_output_time", standart_time);
    pt.put("temperature", temp_unit);
    pt.put("pressure", pressure_unit);
    boost::property_tree::write_json(json_path, pt);
}

void Settings::savePressSettings(std::string press_unit){
    loadSettings();
    pt.put("data_output_time", standart_time);
    pt.put("temperature", temperature_unit);
    pt.put("pressure", press_unit);
    boost::property_tree::write_json(json_path, pt);
}

int Settings::getSensorPollIntervalMs(){
    return int(time);
}
