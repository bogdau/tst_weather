#ifndef __I2C_TK_SDK_I2CIO_RPI_H__
#define __I2C_TK_SDK_I2CIO_RPI_H__

#include "I2CInputOutput.h"

 
#ifndef __I2C_TK_SDK_I2CIO_RPI_PORTS__

#define __I2C_TK_SDK_I2CIO_RPI_PORT0	"/dev/i2c-0"
#define __I2C_TK_SDK_I2CIO_RPI_PORT1 	"/dev/i2c-1"

#endif //__I2C_TK_SDK_I2CIO_RPI_PORTS__


class RPI_I2CInputOutput : public I2CInputOutput {
private:
	uint8_t	_i2cPort;
	int _fileDescriptor;
protected:
public:
	RPI_I2CInputOutput(uint8_t i2cPort);
	virtual ~RPI_I2CInputOutput();
	
	virtual int	setup(uint8_t deviceAddress); //Initialize I²C communications
	virtual int release(); //Release I²C communications
	
	virtual int read(uint8_t *buffer,uint32_t len); //Read data from I²C
	virtual int write(uint8_t *data, uint32_t len); //write data to I²C
};
#endif //__I2C_TK_SDK_I2CIO_RPI_H__