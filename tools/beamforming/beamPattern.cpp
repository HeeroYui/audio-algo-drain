#include <stdio.h>
#include <math.h>
// http://www.labbookpages.co.uk/audio/beamforming/delaySum.html
#define ANGLE_RESOLUTION 500    // Number of angle points to calculate

int main(void) {
	int numElements = 4; // Number of array elements
	double spacing = 0.2; // Element separation in metres
	double freq = 1000.0; // Signal frequency in Hz 
	double speedSound = 343.0; // m/s
	// Iterate through arrival angle points
	for (int32_t aaa=0; aaa<ANGLE_RESOLUTION; aaa++) {
		// Calculate the planewave arrival angle
		double angle = -90.0 + 180.0 * double(aaa) / double(ANGLE_RESOLUTION-1.0);
		double angleRad = M_PI * double(angle) / 180.0;
		double realSum = 0;
		double imagSum = 0;
		// Iterate through array elements
		for (int32_t iii=0; iii<numElements; iii++) {
			// Calculate element position and wavefront delay
			double position = iii * spacing;
			double delay = position * sin(angleRad) / speedSound;
			// Add Wave
			realSum += cos(2.0 * M_PI * freq * delay);
			imagSum += sin(2.0 * M_PI * freq * delay);
		}
		double output = sqrt(realSum * realSum + imagSum * imagSum) / numElements;
		double logOutput = 20 * log10(output);
		if (logOutput < -50) {
			logOutput = -50;
		}
		printf("%d %f %f %f %f\n", a, angle, angleRad, output, logOutput);
	}
	return 0;
}


