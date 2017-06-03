#ifndef ACCEL_FXOS8700_INCLUDED
#define ACCEL_FXOS8700_INCLUDED

#include <stdint.h>

class AccelFXOS8700
{
public:
	enum Error {
		ERROR_NONE = 0,
		ERROR_RANGE_INVALID,

		// The following should be hardware failures.
		// Not wired correctly, likely.
		ERROR_CONNECTION
	};

	AccelFXOS8700() {}

	Error begin(uint8_t range = 2, uint8_t deviceAddress = 0x1f);

	struct V3 {
		float x;
		float y;
		float z;
	};

	Error read(V3* v, float* gravitySquared = 0, float* gravity = 0);

private:
	Error read8(uint8_t reg, uint8_t* value);
	Error write8(uint8_t reg, uint8_t value);

	uint8_t m_range = 2;
	uint8_t m_address = 0x1f; 
	//uint32_t m_micros = 0;
	//V3 m_v3;

	//static const uint32_t MIN_TIME_DELTA = 10000;
};

#endif