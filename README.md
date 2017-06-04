AccelFXOS8700

Very simple library for reading NXP FXOS8077 accelerometers. Notably from 
PJRC and Adafruit.

The goal is code that is much simpler than NXP Motion Sense 
(https://github.com/PaulStoffregen/NXPMotionSense) and less
Adafruit specific than https://github.com/adafruit/Adafruit_FXOS8700.
AccelFXOS8700 also doesn't have additional dependencies (unlike the
Adafruit_FXOS8700 which depends on the Sensor library and structure.)

AccelFXOS8700 is minimal and currently only supports the acceleration.
It would be fairly simple to add the raw gyroscope if needed. If you need
full position data (quaternion, etc.) you do need the Adafruit or MotionSense
library.

Minimal code:

````
#include "accelFXOS8700.h"
#include <Wire.h>
AccelFXOS8700 accel;

void setup() {
	accel.begin(2, 0x01e);
}

void loop() {
	float v[3];
	float g2 = 0;
	float g = 0;

	accel.read(v, &g2, &g);

	Serial.print("vx=");  Serial.print(v[0]);
	Serial.print(" vy="); Serial.print(v[1]);
	Serial.print(" vz="); Serial.print(v[2]);
	Serial.print(" g2="); Serial.print(g2);
	Serial.print(" g=");  Serial.print(g);

	delay(500);
}
````

