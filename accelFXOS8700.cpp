#include "accelFXOS8700.h"
#include <Wire.h>
#include <math.h>

enum
{                                             // DEFAULT    TYPE
  FXOS8700_REGISTER_STATUS          = 0x00,
  FXOS8700_REGISTER_OUT_X_MSB       = 0x01,
  FXOS8700_REGISTER_OUT_X_LSB       = 0x02,
  FXOS8700_REGISTER_OUT_Y_MSB       = 0x03,
  FXOS8700_REGISTER_OUT_Y_LSB       = 0x04,
  FXOS8700_REGISTER_OUT_Z_MSB       = 0x05,
  FXOS8700_REGISTER_OUT_Z_LSB       = 0x06,
  FXOS8700_REGISTER_WHO_AM_I        = 0x0D,   // 11000111   r
  FXOS8700_REGISTER_XYZ_DATA_CFG    = 0x0E,
  FXOS8700_REGISTER_CTRL_REG1       = 0x2A,   // 00000000   r/w
  FXOS8700_REGISTER_CTRL_REG2       = 0x2B,   // 00000000   r/w
  FXOS8700_REGISTER_CTRL_REG3       = 0x2C,   // 00000000   r/w
  FXOS8700_REGISTER_CTRL_REG4       = 0x2D,   // 00000000   r/w
  FXOS8700_REGISTER_CTRL_REG5       = 0x2E,   // 00000000   r/w
  FXOS8700_REGISTER_MSTATUS         = 0x32,
  FXOS8700_REGISTER_MOUT_X_MSB      = 0x33,
  FXOS8700_REGISTER_MOUT_X_LSB      = 0x34,
  FXOS8700_REGISTER_MOUT_Y_MSB      = 0x35,
  FXOS8700_REGISTER_MOUT_Y_LSB      = 0x36,
  FXOS8700_REGISTER_MOUT_Z_MSB      = 0x37,
  FXOS8700_REGISTER_MOUT_Z_LSB      = 0x38,
  FXOS8700_REGISTER_MCTRL_REG1      = 0x5B,   // 00000000   r/w
  FXOS8700_REGISTER_MCTRL_REG2      = 0x5C,   // 00000000   r/w
  FXOS8700_REGISTER_MCTRL_REG3      = 0x5D,   // 00000000   r/w
};

AccelFXOS8700::Error AccelFXOS8700::begin(uint8_t range, uint8_t deviceAddress)
{
	Wire.begin();

	switch (range) {
		case 2:
			write8(FXOS8700_REGISTER_XYZ_DATA_CFG, 0x00);
			m_range = 2;
			break;
		case 4:
			write8(FXOS8700_REGISTER_XYZ_DATA_CFG, 0x01);
			m_range = 4;
			break;
		case 8:
			write8(FXOS8700_REGISTER_XYZ_DATA_CFG, 0x02);
			m_range = 8;
			break;

		default:
			return ERROR_RANGE_INVALID;
	}

	m_address = deviceAddress;

	uint8_t id = 0;
	AccelFXOS8700::Error e = ERROR_NONE;	

	read8(FXOS8700_REGISTER_WHO_AM_I, &id);
  	if (id != 0xC7) return ERROR_CONNECTION;

	// High resolution
	write8(FXOS8700_REGISTER_CTRL_REG2, 0x02);
	// Active, Normal Mode, Low Noise, 100Hz in Hybrid Mode
  	write8(FXOS8700_REGISTER_CTRL_REG1, 0x15);
	// Hybrid Mode, Over Sampling Rate = 16
  	write8(FXOS8700_REGISTER_MCTRL_REG1, 0x1F);
  	// Jump to reg 0x33 after reading 0x06
	e = write8(FXOS8700_REGISTER_MCTRL_REG2, 0x20);

//	m_micros = micros() - MIN_TIME_DELTA;
//	m_v3.x = 1.0f;
//	m_v3.y = 0.0f;
//	m_v3.z = 0.0f;
  	return e;
}


AccelFXOS8700::Error AccelFXOS8700::read8(uint8_t reg, uint8_t* value)
{
	*value = 0;

	Wire.beginTransmission(m_address);
  	Wire.write(reg);

	Wire.endTransmission(0);
 	int n = Wire.requestFrom(int(m_address), 1, 0);
 	if (n != 1) return ERROR_CONNECTION;

    *value = Wire.read();

    return ERROR_NONE;
}


AccelFXOS8700::Error AccelFXOS8700::write8(uint8_t reg, uint8_t value)
{
	Wire.beginTransmission(m_address);
	int written = 0;
    written += Wire.write(reg);
    written += Wire.write(value);
	Wire.endTransmission(m_address);

    return (written == 2) ? ERROR_NONE : ERROR_CONNECTION;
}


AccelFXOS8700::Error AccelFXOS8700::read(V3* v, float* gravitySquared, float* gravity)
{
	Wire.beginTransmission(m_address);
    Wire.write(FXOS8700_REGISTER_STATUS | 0x80);
  	Wire.endTransmission();

  	Wire.requestFrom(int(m_address), 13);

    uint8_t status = Wire.read();
    uint8_t axhi = Wire.read();
    uint8_t axlo = Wire.read();
    uint8_t ayhi = Wire.read();
    uint8_t aylo = Wire.read();
    uint8_t azhi = Wire.read();
    uint8_t azlo = Wire.read();
    for(int i=0; i<6; ++i) {
    	Wire.read();
    }
    (void) status;
//    uint8_t mxhi = Wire.read();
//    uint8_t mxlo = Wire.read();
//    uint8_t myhi = Wire.read();
//    uint8_t mylo = Wire.read();
//    uint8_t mzhi = Wire.read();
//    uint8_t mzlo = Wire.read();
 
  	int16_t accelX = (int16_t)((axhi << 8) | axlo) >> 2;
  	int16_t accelY = (int16_t)((ayhi << 8) | aylo) >> 2;
  	int16_t accelZ = (int16_t)((azhi << 8) | azlo) >> 2;
 
 /*
 	static const float ACCEL_2G = 0.000244F;
	static const float ACCEL_4G = 0.000488F;
	static const float ACCEL_8G = 0.000976F;
	static const float GRAVITY = 9.82;*/

	float convert = 1;
	static const float CONVERT_ACCEL =  0.0001220703125f;  // = 1/8192
	switch(m_range) {
		case 2:	
			convert = 2.0f * CONVERT_ACCEL;
			break;
		case 4:
			convert = 4.0f * CONVERT_ACCEL;
			break;
		default:
			convert = 8.0f * CONVERT_ACCEL;
			break;
	}
	v->x = float(accelX) * convert;
	v->y = float(accelY) * convert;
	v->z = float(accelZ) * convert;

	float g2 = 1;

	if (gravitySquared || gravity) {
		g2 = v->x * v->x + v->y * v->y + v->z * v->z;
	}
	if (gravitySquared) 
		*gravitySquared = g2;
	if (gravity) 
		*gravity = sqrt(g2);

	return ERROR_NONE;
}
