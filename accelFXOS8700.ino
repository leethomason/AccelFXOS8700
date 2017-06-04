#include "accelFXOS8700.h"
#include <Wire.h>
AccelFXOS8700 accel;

void setup() {
	Serial.begin(19200);
	while(!Serial) delay(100);

	Serial.println("Accel begin.");
	AccelFXOS8700::ErrorCode e = accel.begin(4, AccelFXOS8700::ADDRESS_PROP_SHIELD);

	Serial.print("begin e=");
	Serial.println((int)e);
}

void loop() {
	float v[3];
	float g2 = 0;
	float g = 0;

	uint32_t t0 = micros();
	AccelFXOS8700::ErrorCode rc0 = accel.read(v, 0, &g);
	uint32_t t1 = micros();
	AccelFXOS8700::ErrorCode rc1 = accel.read(v, &g2, &g);
	uint32_t t2 = micros();

	Serial.print("vx= "); Serial.print(v[0]);
	Serial.print(" vy="); Serial.print(v[1]);
	Serial.print(" vz="); Serial.print(v[2]);
	Serial.print(" g2="); Serial.print(g2);
	Serial.print(" g="); Serial.print(g);
	Serial.print(" rc0="); Serial.print((int)rc0);
	Serial.print(" rc1="); Serial.print((int)rc1);
	Serial.print(" micros="); Serial.print(t1 - t0);
	Serial.print(" micros="); Serial.println(t2 - t1);

	if (rc0 != AccelFXOS8700::SUCCESS || rc1 != AccelFXOS8700::SUCCESS) {
		while(true);
	}

	delay(500);
}
