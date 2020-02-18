#include "mbed.h"
#include "L3G4200D.h"
#include "HMC5883L.h"
#define M_PI 3.14159

Serial pc(USBTX,USBRX);
L3G4200D gyro(D14, D15);
HMC5883L magneto(D2, D8);

Timer timer;

DigitalOut Vcc(A0);
AnalogIn xIn(A1);
AnalogIn yIn(A2);
AnalogIn zIn(A3);
DigitalOut Gnd(A4);

int g[3] = {0,0,0};
float filterWeight = 0.1;

int main()
{
	Vcc = 1;
	Gnd = 0;
	timer.start();
	
	float roll = 0;
	float pitch = 0;

	while(true)
    {
		float aika = timer.read_ms();        
		timer.reset();
		
		gyro.read(g);
		
		// Vakiotason korjaukset
		//g[0] = g[0] + 0;
		g[1] = g[1] + 11;
		//g[2] = g[2] + 2;

		float ax = 98.585 * xIn.read() - 48.307;
		float ay = 95.243 * yIn.read() - 48.288;
		float az = 101.1 * zIn.read() - 51.056;
				
		pitch += g[1] * (aika / 1000) * 0.071174;
		roll += g[0] * (aika / 1000) * 0.071174;
				
		float accPitch = 180 * atan (ax/sqrt(ay*ay + az*az))/M_PI;
		float accRoll = 180 * atan (ay/sqrt(ax*ax + az*az))/M_PI;
				
		pitch = (1-filterWeight) * pitch + filterWeight * accPitch;
	    roll = (1-filterWeight) * roll + filterWeight * accRoll;
			
		pc.printf("%.2fs%.2f\n", pitch, roll);


	}
}
