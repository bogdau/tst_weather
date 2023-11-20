#include <stdio.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include "BMP280.h"

BMP280::BMP280(uint8_t address, I2CInputOutput *i2cIO) : 
    I2CDevice(address,i2cIO),
    _temperatureOversampling(SKIPPED),
    _pressureOversampling(SKIPPED),
    _operatingMode(SLEEP),
    _fineTemperature(0)
{
}

BMP280::~BMP280(){

}

int BMP280::initialize(){
	I2CDevice::initialize();
	//Read chip id
	this->_chipId = this->readChipId();

	//Read calibration
	this->_calibration = this->readCalibration();

	return 0;
}

uint8_t BMP280::readChipId()
{
	uint8_t buffer[1] = {__BMP280_REGISTER_CHIP_ID};
	I2CDevice::write(buffer,1);
	I2CDevice::waitForResponse();
	I2CDevice::read(buffer,1);
	return buffer[0];
}

void BMP280::reset(bool completeReset)
{
	uint8_t buffer[2] = {__BMP280_REGISTER_RESET, static_cast<uint8_t>(completeReset ? __BMP280_CONST_COMPLETE_RESET : 0x00)};

	I2CDevice::write(buffer,2);

	this->_temperatureOversampling = SKIPPED;
	this->_pressureOversampling = SKIPPED;
	this->_operatingMode = SLEEP;
}

BMP280_STATUS BMP280::status()
{
	uint8_t buffer[1] = {__BMP280_REGISTER_STATUS};
	I2CDevice::write(buffer,1);
	I2CDevice::waitForResponse();
	I2CDevice::read(buffer,1);

	BMP280_STATUS status;
	status.measuring 	= (buffer[0] & 0x04) > 0;
	status.imUpdate 	= (buffer[0] & 0x01) > 0;
	return status;
}

void BMP280::controlMeasure(BMP280_OVERSAMPLING pressureOversampling, BMP280_OVERSAMPLING temperatureOversampling, BMP280_OPERATING_MODE operatingMode)
{
	this->_temperatureOversampling = pressureOversampling;
	this->_pressureOversampling = temperatureOversampling;
	this->_operatingMode = operatingMode;


	uint8_t value = ((pressureOversampling & 0x07) << 5) | 
					((temperatureOversampling & 0x07) << 2) |
					(operatingMode & 0x03);

	uint8_t buffer[2] = {__BMP280_REGISTER_CTRL_MEASURE, value};
	I2CDevice::write(buffer,2);
}

void BMP280::config()
{

}

double BMP280::pressure()
{
	uint8_t reg[1] = {__BMP280_REGISTER_PRESSURE_MSB};
	I2CDevice::write(reg,1);

	uint8_t buffer[3];
	memset(&buffer[0],0,sizeof(uint8_t)*3);
	I2CDevice::read(buffer,3);
	int32_t rawPressure = (int32_t) (((uint32_t) buffer[0]) << 12) | (((uint32_t) buffer[1]) << 4) | (((uint32_t)buffer[2]) >> 4);
	return this->compensatePressure(rawPressure);
}

double BMP280::compensatePressure(int32_t rawValue){
	
	double x1 = ((double) this->_fineTemperature / 2.0) - 64000.0;
	double x2 = x1 * x1 * ((double) this->_calibration.digP6) / 32768.0;

	x2 += x1 * ((double) this->_calibration.digP5) * 2.0;
	x2 = (x2  / 4.0) + (((double) this->_calibration.digP4) * 65536.0); 
	x1 = (((double)  this->_calibration.digP3) * x1 * x1 / 524288.0 + ((double) this->_calibration.digP2) * x1) / 524288.0;
	x1 = (1.0 + x1 / 32768.0) * ((double) this->_calibration.digP1);

	double pressure = 1048576.0 - (double)rawValue;
	if (x1 == 0)
	{
		return 0.0;
	}

	pressure = (pressure - (x2 / 4096.0))  * (6250.0 / x1);
	x1 = ((double) this->_calibration.digP9) * pressure * pressure / 2147483648.0;
	x2 = pressure * ((double) this->_calibration.digP8) / 32768.0;
	pressure = pressure + (x1 + x2 + ((double) this->_calibration.digP7)) / 16.0;
 	return pressure;
}

double BMP280::temperature()
{
	uint8_t reg[1] = {__BMP280_REGISTER_TEMPERATURE_MSB};
	I2CDevice::write(reg,1);

	uint8_t buffer[3];
	memset(&buffer[0],0,sizeof(uint8_t)*3);
	I2CDevice::read(buffer,3);
	int32_t rawTemp = (int32_t) (((uint32_t) buffer[0]) << 12) | (((uint32_t) buffer[1]) << 4) | (((uint32_t)buffer[2]) >> 4);
	return this->compensateTemperature(rawTemp);
}

double BMP280::compensateTemperature(int32_t rawValue){

	double x1 = ( ((double) rawValue) / 16384.0 - ((double) this->_calibration.digT1) / 1024.0 ) *
				((double)this->_calibration.digT2);

	double x2 = ( ((double) rawValue) / 131072.0 - ((double) this->_calibration.digT1) / 8192.0 ) *
				( ((double) rawValue) / 131072.0 - ((double) this->_calibration.digT1) / 8192.0 ) * 
				((double)this->_calibration.digT3);

	this->_fineTemperature = (int32_t) (x1 + x2);
	double temperature = (x1 + x2) / 5120.0;
	return temperature;
}

void BMP280::forceReadTemperaturePresure(double *temperature, double *pressure)
{
	this->controlMeasure(this->_pressureOversampling,this->_temperatureOversampling,FORCED);

	usleep(this->computeWaitingTime() * 1000);

	uint8_t reg[1] = {__BMP280_REGISTER_PRESSURE_MSB};
	I2CDevice::write(reg,1);
	uint8_t buffer[6];
	memset(&buffer[0],0,sizeof(uint8_t)*6);
	I2CDevice::read(buffer,6);
	if (pressure != 0)
	{
		int32_t rawPressure = (int32_t) (((uint32_t) buffer[0]) << 12) | (((uint32_t) buffer[1]) << 4) | (((uint32_t)buffer[2]) >> 4);
		*pressure= this->compensatePressure(rawPressure);
	}
	if (temperature != 0)
	{
		int32_t rawTemp = (int32_t) (((uint32_t) buffer[3]) << 12) | (((uint32_t) buffer[4]) << 4) | (((uint32_t)buffer[5]) >> 4);
		*temperature =  this->compensateTemperature(rawTemp);
	}
}

BMP280_CALIBRATION BMP280::readCalibration()
{
	uint8_t buffer[1] = {	__BMP280_REGISTER_CALIBRATION_DIG_TEMPERATURE_T1_LSB};
	I2CDevice::write(buffer,1);
	uint8_t data[24];
	memset(&data[0], 0, sizeof(uint8_t)*24);
	I2CDevice::read(data,24);

	BMP280_CALIBRATION calibration;
	calibration.digT1 = (uint16_t)((((uint16_t)((uint8_t)data[1])) 	<< 8) | data[0]);
	calibration.digT2 = (int16_t)((((int16_t)((int8_t)data[3])) 	<< 8) | data[2]);
	calibration.digT3 = (int16_t)((((int16_t)((int8_t)data[5])) 	<< 8) | data[4]);

	calibration.digP1 = (uint16_t)((((uint16_t)((uint8_t)data[7])) 	<< 8) | data[6]);
	calibration.digP2 = (int16_t)((((int16_t)((int8_t)data[9])) 	<< 8) | data[8]);
	calibration.digP3 = (int16_t)((((int16_t)((int8_t)data[11])) 	<< 8) | data[10]);
	calibration.digP4 = (int16_t)((((int16_t)((int8_t)data[13])) 	<< 8) | data[12]);
	calibration.digP5 = (int16_t)((((int16_t)((int8_t)data[15])) 	<< 8) | data[14]);
	calibration.digP6 = (int16_t)((((int16_t)((int8_t)data[17])) 	<< 8) | data[16]);
	calibration.digP7 = (int16_t)((((int16_t)((int8_t)data[19])) 	<< 8) | data[18]);
	calibration.digP8 = (int16_t)((((int16_t)((int8_t)data[21])) 	<< 8) | data[20]);
	calibration.digP9 = (int16_t)((((int16_t)((int8_t)data[23])) 	<< 8) | data[22]);

	return calibration;
}

uint8_t BMP280::computeWaitingTime()
{
	uint8_t wt = (__BMP280_T_INIT_MAX + __BMP280_T_MEASURE_PER_OSRS_MAX * (
					((1 << this->_temperatureOversampling) >> 1) +
					((1 << this->_pressureOversampling) >> 1)) +
					((this->_pressureOversampling > 0) ? __BMP280_T_SETUP_PRESSURE_MAX : 0) +
					15) / 16;
	return wt;
}
