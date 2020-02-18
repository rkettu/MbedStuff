
#include "mbed.h"
#include "HMC5883L.h"
#define M_PI 3.14159

HMC5883L magneto(D2, D8);
Serial pc(USBTX,USBRX);
int16_t m[3] = {0};

int main()
{
	// Calculating initial heading
	magneto.getXYZ(m);
	float initialH = atan2((float)m[2], (float)m[0]);
	initialH += 0.1937;
	if(initialH < 0)    initialH += 2*M_PI;
	if(initialH > 2*M_PI) initialH -= 2*M_PI;
	initialH *= 180/M_PI;
	
	while(true)
	 {
		magneto.getXYZ(m);
		
		float heading = atan2((float)m[2], (float)m[0]);
		heading += 0.1937;
		
		if(heading < 0)    heading += 2*M_PI;
 
		if(heading > 2*M_PI) heading -= 2*M_PI;
		
		heading *= 180/M_PI;
		heading -= initialH;	// Substracting initial heading gives angle from start position
		
		if(heading > 180) 
		{
			heading = -(360 - heading);
		}
		
		//pc.printf("%fx %fy %fz \n", (float)m[0], (float)m[2], (float)m[1]);
		pc.printf("%f\n", heading);
	}
}
