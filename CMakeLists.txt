cmake_minimum_required(VERSION 3.16)
project(tst_weather)

set(CMAKE_CXX_STANDARD 14)

add_library(bosch_bmp280 STATIC driver/bmp280.c)
target_include_directories(bosch_bmp280 PUBLIC driver/)

add_library(raspberry_bmp280 STATIC include/raspberry_bmp280.cpp)
target_include_directories(raspberry_bmp280 PUBLIC include)
target_link_libraries(raspberry_bmp280 PUBLIC bosch_bmp280)

add_executable(tst_weather src/main.cpp)
target_include_directories(tst_weather PUBLIC src/)
target_sources(tst_weather PUBLIC src/data_output.cpp
                            src/data_reading.cpp
                            src/data_manager.cpp
                            src/json.cpp)
target_link_libraries(tst_weather PUBLIC raspberry_bmp280)