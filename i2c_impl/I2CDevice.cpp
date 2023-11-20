#include "I2CDevice.h"

I2CDevice::I2CDevice(uint8_t address, I2CInputOutput *i2cIO) :
	_deviceAddress(address)
{
	if (i2cIO == 0)
	{

	}
	this->_i2cIO = i2cIO;
}
int I2CDevice::initialize()
{
	this->setup();
	return 0;
}

void I2CDevice::setup()
{
	if (this->_i2cIO != 0)
	{
		this->_i2cIO->setup(_deviceAddress);
	}
}

int I2CDevice::read(uint8_t *buffer, uint32_t len){
	if (this->_i2cIO == 0)
	{
		return I2CIO_ERROR_ERROR;
	}

	return this->_i2cIO->read(buffer,len);
}

int I2CDevice::write(uint8_t *data, uint32_t len){
	if (this->_i2cIO == 0)
	{
		return I2CIO_ERROR_ERROR;
	}

	return this->_i2cIO->write(data,len);
}