#include <stdio.h>
#include <math.h>
// http://www.labbookpages.co.uk/audio/beamforming/delaySum.html

// Number of freq points to calculate
#define FREQ_RESOLUTION 500
// Number of angle points to calculate
#define ANGLE_RESOLUTION 500

int main(void) {
	int numElements = 4; // Number of array elements
	double spacing = 0.2; // Element separation in metre
	double speedSound = 343.0; // m/s
	// Iterate through arrival angle points
	for (int32_t fff=0; fff<FREQ_RESOLUTION; ++fff) {
		double freq = 10000.0 * double(fff) / double(FREQ_RESOLUTION-1);
		for (int32_t aaa=0; aaa<ANGLE_RESOLUTION; ++aaa) {
			// Calculate the planewave arrival angle
			double angle = -90.0 + 180.0 * double(aaa) / double(ANGLE_RESOLUTION-1);
			double angleRad = M_PI * double(angle) / 180.0;
			double realSum = 0;
			double imagSum = 0;
			// Iterate through array elements
			for (iii=0; iii<numElements; ++iii) {
				// Calculate element position and wavefront delay
				double position = double(iii) * spacing;
				double delay = position * sin(angleRad) / speedSound;
				// Add Wave
				realSum += cos(2.0 * M_PI * freq * delay);
				imagSum += sin(2.0 * M_PI * freq * delay);
			}
			double output = sqrt(realSum * realSum + imagSum * imagSum) / numElements;
			double logOutput = 20.0 * log10(output);
			if (logOutput < -50.0) {
				logOutput = -50.0;
			}
			printf("%f %f %f\n", angle, freq, logOutput);
		}
		printf("\n");
	}
	return 0;
}