#ifndef ACCEL_FXOS8700_INCLUDED
#define ACCEL_FXOS8700_INCLUDED

#include <stdint.h>

class AccelFXOS8700
{
public:
	enum ErrorCode {
		SUCCESS = 0,
		ERROR_RANGE_INVALID,

		// The following should be hardware failures.
		// Not wired correctly, likely.
		ERROR_CONNECTION
	};

	AccelFXOS8700() {}

	ErrorCode begin(uint8_t range = 2, uint8_t deviceAddress = 0x1f);

	ErrorCode read(float* v, float* gravitySquared = 0, float* gravity = 0);

private:
	ErrorCode read8(uint8_t reg, uint8_t* value);
	ErrorCode write8(uint8_t reg, uint8_t value);

	uint8_t m_range = 2;
	uint8_t m_address = 0x1f; 
};

#endif