#include "accelFXOS8700.h"
#include <Wire.h>

AccelFXOS8700 accel;

void setup() {
	Serial.begin(19200);
	while(!Serial) delay(100);

	Serial.println("Accel begin.");
	AccelFXOS8700::Error e = accel.begin(2, 0x01e);

	Serial.print("begin e=");
	Serial.println((int)e);
}

void loop() {
	AccelFXOS8700::V3 v;
	float g2 = 0;
	float g = 0;

	accel.read(&v, &g2, &g);

	Serial.print("vx= "); Serial.print(v.x);
	Serial.print(" vy="); Serial.print(v.y);
	Serial.print(" vz="); Serial.print(v.z);
	Serial.print(" g2="); Serial.print(g2);
	Serial.print(" g="); Serial.println(g);

	delay(500);
}
