extern "C" {
	#include <stdio.h>
	#include <math.h>
	#include <stdint.h>
}
// http://www.labbookpages.co.uk/audio/beamforming/delaySum.html

// Number of freq points to calculate
#define FREQ_RESOLUTION 500
// Number of angle points to calculate
#define ANGLE_RESOLUTION 1000

// ./binary > freqResp.dat
// gnuplot
// gnuplot> call 'freqResp.gnuplot'

#include <etk/etk.hpp>
#include <test-debug/debug.hpp>
#include <etk/os/FSNode.hpp>
#include <etk/math/Vector3D.hpp>
#include <etk/math/Matrix4.hpp>

const double speedSound = 340.29; // m/s

class Direction {
	public:
		double radius; //!< radius distance of the source
		double angleAlpha; //!< angle on the XY plane, 0 is to the right and turning to the left is positive (clock orientation)
		double angleEtha; //!< angle on the XZ plane, 0 is directly ahead, turning upwards is positive
		Direction(double _alpha, double _etha, double _radius) {
			radius = _radius;
			angleAlpha = _alpha;
			angleEtha = _etha;
		}
};


double calculateDelay(vec3 _pos, Direction _dir) {
	//convert direction in position:
	vec3 posSource(0.0,0.0,_dir.radius);
	//mat4 matrix = etk::matRotate(vec3(0,1,0), _dir.angleAlpha);// * etk::matRotate(vec3(1,0,0), _dir.angleEtha);
	//vec3 pos = matrix * posSource;
	vec3 pos = posSource.rotate(vec3(0,1,0), _dir.angleAlpha);
	double delay = (_pos-pos).length();
	double delayBase = pos.length();
	TEST_DEBUG("posSource : src=" << pos << " to dst=" << _pos << " distance=" << delay);
	TEST_DEBUG("   delta = " << delay - delayBase << " angle=" << _dir.angleAlpha*180/M_PI);
	TEST_DEBUG("   theoric = " << _pos.x() * sin(_dir.angleAlpha));
	return (delay - delayBase) / speedSound;
}

#if 0
	int32_t posCount = 4;
	vec3 positions[4] = {
		{ 0.0, 0, 0},
		{ 0.2, 0, 0},
		{ 0.4, 0, 0},
		{ 0.6, 0, 0},
	};
#endif

#if 0
	int32_t posCount = 4;
	vec3 positions[4] = {
		{ -0.3, 0, 0},
		{ -0.1, 0, 0},
		{ 0.1, 0, 0},
		{ 0.3, 0, 0},
	};
#endif

#if 0
	int32_t posCount = 6;
	vec3 positions[6] = {
		{ -0.1, 0,  0.1},
		{ -0.1, 0, -0.1},
		{ -0.1, 0, -0.2},
		{  0.1, 0, -0.2},
		{  0.1, 0, -0.1},
		{  0.1, 0,  0.1},
	};
#endif

#if 0
	int32_t posCount = 9;
	vec3 positions[9] = {
		{ -0.3, 0,  0.1},
		{ -0.3, 0, -0.1},
		{ -0.3, 0, -0.3},
		{ -0.1, 0,  0.1},
		{ -0.1, 0, -0.1},
		{ -0.1, 0, -0.3},
		{  0.1, 0, -0.3},
		{  0.1, 0, -0.1},
		{  0.1, 0,  0.1},
	};
#endif

#if 0
	int32_t posCount = 4;
	vec3 positions[4] = {
		{ -0.05,  0.05, 0},
		{ -0.05, -0.05, 0},
		{  0.05, -0.05, 0},
		{  0.05,  0.05, 0},
	};
#endif


#if 0
	int32_t posCount = 4;
	vec3 positions[4] = {
		{-0.06, 0, 0},
		{-0.02, 0, 0},
		{ 0.02, 0, 0},
		{ 0.06, 0, 0},
	};
#endif

#if 1
	int32_t posCount = 4;
	vec3 positions[4] = {
		{-0.028, 0,  0.0},
		{-0.028, 0, -0.07},
		{ 0.028, 0, -0.07},
		{ 0.028, 0,  0.0},
	};
#endif
#if 0
	int32_t posCount = 6;
	vec3 positions[6] = {
		{-0.028, 0,  0.0},
		{-0.010, 0,  0.0},
		{-0.028, 0, -0.07},
		{ 0.028, 0, -0.07},
		{ 0.010, 0,  0.0},
		{ 0.028, 0,  0.0},
	};
#endif

#if 0
	int32_t posCount = 6;
	vec3 positions[6] = {
		{ 0.1, 0.0,  0.0},
		{ 0.0, 0.1,  0.0},
		{-0.1, 0.0,  0.0},
		{ 0.0,-0.1,  0.0},
		{ 0.05, 0.08, -0.1},
		{-0.05, 0.08, -0.1},
	};
#endif

const double distanceEar = 3;

int main(int argc, const char *argv[]) {
	etk::init(argc, argv);
	TEST_INFO("start calculation");
	etk::FSNode node("freqResp.dat");
	node.fileOpenWrite();
	double basicApplyDelay[posCount];
	for (int32_t iii=0; iii<posCount; iii++) {
		basicApplyDelay[iii] = 0.0f;
		basicApplyDelay[iii] = calculateDelay(positions[iii], Direction(0.0f/*M_PI/4*/, 0.0, distanceEar));
		TEST_INFO("single delay: " << iii << " pos=" << positions[iii] << " value=" << basicApplyDelay[iii] << " sample 48k=" << basicApplyDelay[iii]*48000.0);
	}
	// Iterate through arrival angle points
	for (int32_t fff=0; fff<FREQ_RESOLUTION; ++fff) {
		double freq = 10000.0 * double(fff) / double(FREQ_RESOLUTION-1);
		for (int32_t aaa=0; aaa<ANGLE_RESOLUTION; ++aaa) {
			// Calculate the planewave arrival angle
			double angle = -180.0 + 360.0 * double(aaa) / double(ANGLE_RESOLUTION-1);
			double angleRad = M_PI * double(angle) / 180.0;
			double realSum = 0;
			double imagSum = 0;
			// Iterate through array elements
			for (int32_t iii=0; iii<posCount; ++iii) {
				// Calculate element position and wavefront delay
				double delay = calculateDelay(positions[iii], Direction(angleRad, 0.0, distanceEar));
				// Add Wave
			realSum += cos(2.0 * M_PI * freq * (delay-basicApplyDelay[iii]));
			imagSum += sin(2.0 * M_PI * freq * (delay-basicApplyDelay[iii]));
			}
			double output = sqrt(realSum * realSum + imagSum * imagSum) / posCount;
			double logOutput = 20.0 * log10(output);
			if (logOutput < -50.0) {
				logOutput = -50.0;
			}
			char ploppp[4096];
			sprintf(ploppp, "%f %f %f\n", angle, freq, logOutput);
			node.filePuts(ploppp);
		}
		node.filePuts("\n");
	}
	node.fileClose();
	return 0;
}