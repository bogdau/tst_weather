#include "I2CInputOutput.h"

I2CInputOutput::I2CInputOutput() :
	_deviceAddress(0) 
{

}

I2CInputOutput::~I2CInputOutput() {
}

void I2CInputOutput::setDeviceAddress(uint8_t deviceAddress) {
	this->_deviceAddress = deviceAddress;
}

uint8_t I2CInputOutput::getDeviceAddress() {
	return this->_deviceAddress;
}

void I2CInputOutput::waitForResponse()
{
	usleep(I2CIO_WAIT_TIME);
}