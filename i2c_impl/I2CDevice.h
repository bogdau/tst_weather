#include <stdint.h>

#ifndef __I2C_TK_SDK_ABSTRACT_DEVICE_H__
#define __I2C_TK_SDK_ABSTRACT_DEVICE_H__

#include "../i2c_impl/I2CInputOutput.h"
class I2CDevice {
private:
	uint8_t	_deviceAddress;	//unsigned short for device I²C Address
	I2CInputOutput *_i2cIO; //Underlaying I2CInputOutput instance
	void setup();
protected:
	int read(uint8_t *buffer,uint32_t len); //Read data from underlaying I²C IO instance
	int write(uint8_t *data, uint32_t len); //write data to underlaying I²C IO instance

public:
	I2CDevice(uint8_t address, I2CInputOutput *i2cIO);
	virtual ~I2CDevice(){};
	void waitForResponse(){ this->_i2cIO->waitForResponse(); };

	virtual int initialize(); //Send initialization commands to device

	
};

#endif //__I2C_TK_SDK_ABSTRACT_DEVICE_H__