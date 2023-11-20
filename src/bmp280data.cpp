#include "include/bmp280data.h"
#include <iostream>
#include <memory>

BMP280Data::BMP280Data(){
        RPI_IO = new RPI_I2CInputOutput(1);
        bmp280 = new BMP280(0x76,RPI_IO);
        init();
}

int BMP280Data::init(){
    initializeSignalCallback();
	std::cout << "BMP280 Test" << std::endl;

//	RPI_IO = new RPI_I2CInputOutput(1);
//	bmp280 = new BMP280(0x76,RPI_IO);

	std::cout << "Full reset of the Device" << std::endl;
	bmp280->reset(true);
	std::cout << "Initializing Device" << std::endl;
	bmp280->initialize();

	std::cout << "Chip ID: " <<  std::hex << std::setw(2) <<  (int) bmp280->getChipId() << std::endl;

	BMP280_STATUS status = bmp280->status();
	std::cout << "Device status: measuring ?"<< status.measuring << ", imUpdate? " << status.imUpdate << std::endl;
	
	BMP280_CALIBRATION calibration = bmp280->readCalibration();

	std::cout << "Reading Calibration: " << std::endl;
	std::cout << " T1: " <<  std::hex << std::setw(2) <<  (uint16_t) calibration.digT1;
	std::cout << " T2: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digT2;
	std::cout << " T3: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digT3;

    std::cout << std::endl;

	std::cout << " P1: " <<  std::hex << std::setw(2) <<  (uint16_t) calibration.digP1;
	std::cout << " P2: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP2;
	std::cout << " P3: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP3;
	std::cout << std::endl;
	std::cout << " P4: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP4;
	std::cout << " P5: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP5;
	std::cout << " P6: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP6;
	std::cout << std::endl;
	std::cout << " P7: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP7;
	std::cout << " P8: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP8;
	std::cout << " P9: " <<  std::hex << std::setw(2) <<  (int16_t) calibration.digP9;
	std::cout << std::endl;

	std::cout << "Looping through reading T° & hPa" << std::endl;
    
    std::cout << "Press CTRL+C to quit" << std::endl;
    bool normalMode = false;

	bmp280->controlMeasure(OVERSAMPLING_16X,OVERSAMPLING_16X,NORMAL);

    return 0;
}

void BMP280Data::initializeSignalCallback(){
    struct sigaction sigIntHandler;

//	sigIntHandler.sa_handler = signal_callback;
	sigemptyset(&sigIntHandler.sa_mask);
	sigIntHandler.sa_flags = 0;

	sigaction(SIGINT, &sigIntHandler, NULL);
}


void  BMP280Data::signal_callback(int s){
    std::cout << "Caught signal" << s << std::endl;
    exit(1); 
}


double BMP280Data::readTemp(){
    return bmp280->temperature();
}

double BMP280Data::readPressure(){
    return bmp280->pressure();
}
