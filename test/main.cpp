/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <test-debug/debug.hpp>
#include <etk/etk.hpp>
#include <audio/algo/drain/Equalizer.hpp>
#include <etk/os/FSNode.hpp>
#include <chrono>
#include <thread>



class Performance {
	private:
		std::chrono::steady_clock::time_point m_timeStart;
		std::chrono::steady_clock::time_point m_timeStop;
		std::chrono::nanoseconds m_totalTimeProcessing;
		std::chrono::nanoseconds m_minProcessing;
		std::chrono::nanoseconds m_maxProcessing;
		int32_t m_totalIteration;
	public:
		Performance() :
		  m_totalTimeProcessing(0),
		  m_minProcessing(99999999999999LL),
		  m_maxProcessing(0),
		  m_totalIteration(0) {
			
		}
		void tic() {
			m_timeStart = std::chrono::steady_clock::now();
		}
		void toc() {
			m_timeStop = std::chrono::steady_clock::now();
			std::chrono::nanoseconds time = m_timeStop - m_timeStart;
			m_minProcessing = etk::min(m_minProcessing, time);
			m_maxProcessing = etk::max(m_maxProcessing, time);
			m_totalTimeProcessing += time;
			m_totalIteration++;
			
		}
		
		std::chrono::nanoseconds getTotalTimeProcessing() {
			return m_totalTimeProcessing;
		}
		std::chrono::nanoseconds getMinProcessing() {
			return m_minProcessing;
		}
		std::chrono::nanoseconds getMaxProcessing() {
			return m_maxProcessing;
		}
		int32_t getTotalIteration() {
			return m_totalIteration;
		}
		
};

float performanceEqualizerType(audio::format _type) {
	etk::Vector<float> input;
	input.resize(1024, 0);
	etk::Vector<float> output;
	output.resize(input.size()*10, 0);
	double sampleRate = 48000;
	{
		double phase = 0;
		double baseCycle = 2.0*M_PI/sampleRate * 480.0;
		for (int32_t iii=0; iii<input.size(); iii++) {
			input[iii] = cos(phase) * 5.0;
			phase += baseCycle;
			if (phase >= 2*M_PI) {
				phase -= 2*M_PI;
			}
		}
	}
	TEST_INFO("Start Equalizer (1 biquad) ... " << _type);
	Performance perfo;
	audio::algo::drain::Equalizer algo;
	// configure in float
	algo.init(48000, 1, _type);
	// add basic biquad ...
	algo.addBiquad(0.01, 0.02, 0.03, 0.05, 0.06);
	// set 1024 test
	for (int32_t iii=0; iii<1024; ++iii) {
		perfo.tic();
		size_t sizeOut = output.size();
		algo.process(&output[0], &input[0], input.size());
		perfo.toc();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	TEST_INFO("    blockSize=" << input.size() << " sample");
	TEST_INFO("    min < avg < max =" << perfo.getMinProcessing().count() << "ns < "
	                                  << perfo.getTotalTimeProcessing().count()/perfo.getTotalIteration() << "ns < "
	                                  << perfo.getMaxProcessing().count() << "ns ");
	float avg = (float(((perfo.getTotalTimeProcessing().count()/perfo.getTotalIteration())*sampleRate)/double(input.size()))/1000000000.0)*100.0;
	TEST_INFO("    min < avg < max= " << (float((perfo.getMinProcessing().count()*sampleRate)/double(input.size()))/1000000000.0)*100.0 << "% < "
	                                  << avg << "% < "
	                                  << (float((perfo.getMaxProcessing().count()*sampleRate)/double(input.size()))/1000000000.0)*100.0 << "%");
	TEST_PRINT("type=" << _type << ": " << avg << "%");
	return avg;
}

void performanceEqualizer() {
	performanceEqualizerType(audio::format_double);
	performanceEqualizerType(audio::format_float);
	performanceEqualizerType(audio::format_int8);
	performanceEqualizerType(audio::format_int8_on_int16);
	performanceEqualizerType(audio::format_int16);
	performanceEqualizerType(audio::format_int16_on_int32);
	performanceEqualizerType(audio::format_int32);
	performanceEqualizerType(audio::format_int32_on_int64);
	performanceEqualizerType(audio::format_int64);
}

int main(int _argc, const char** _argv) {
	// the only one init for etk:
	etk::init(_argc, _argv);
	etk::String inputName = "";
	etk::String outputName = "output.raw";
	bool performance = false;
	bool perf = false;
	int64_t sampleRateIn = 48000;
	int64_t sampleRateOut = 48000;
	int32_t nbChan = 1;
	int32_t quality = 4;
	etk::String test = "";
	for (int32_t iii=0; iii<_argc ; ++iii) {
		etk::String data = _argv[iii];
		if (etk::start_with(data,"--in=")) {
			inputName = &data[5];
		} else if (etk::start_with(data,"--out=")) {
			outputName = &data[6];
		} else if (data == "--performance") {
			performance = true;
		} else if (data == "--perf") {
			perf = true;
		} else if (etk::start_with(data,"--test=")) {
			data = &data[7];
			sampleRateIn = etk::string_to_int32_t(data);
		} else if (etk::start_with(data,"--sample-rate-in=")) {
			data = &data[17];
			sampleRateIn = etk::string_to_int32_t(data);
		} else if (etk::start_with(data,"--sample-rate-out=")) {
			data = &data[18];
			sampleRateOut = etk::string_to_int32_t(data);
		} else if (etk::start_with(data,"--nb=")) {
			data = &data[5];
			nbChan = etk::string_to_int32_t(data);
		} else if (etk::start_with(data,"--quality=")) {
			data = &data[10];
			quality = etk::string_to_int32_t(data);
		} else if (    data == "-h"
		            || data == "--help") {
			TEST_PRINT("Help : ");
			TEST_PRINT("    ./xxx --fb=file.raw --mic=file.raw");
			TEST_PRINT("        --in=YYY.raw            input file");
			TEST_PRINT("        --out=zzz.raw           output file");
			TEST_PRINT("        --performance           Generate signal to force algo to maximum process time");
			TEST_PRINT("        --perf                  Enable performence test (little slower but real performence test)");
			TEST_PRINT("        --test=XXXX             some test availlable ...");
			TEST_PRINT("            EQUALIZER          Test resampling data 16 bit mode");
			TEST_PRINT("                --sample-rate=XXXX   Input signal sample rate (default 48000)");
			
			exit(0);
		}
	}
	// PERFORMANCE test only ....
	if (performance == true) {
		performanceEqualizer();
		return 0;
	}
	if (test == "EQUALIZER") {
		/*
		TEST_INFO("Start resampling test ... ");
		if (inputName == "") {
			TEST_ERROR("Can not Process missing parameters...");
			exit(-1);
		}
		TEST_INFO("Read input:");
		etk::Vector<int16_t> inputData = etk::FSNodeReadAllDataType<int16_t>(inputName);
		TEST_INFO("    " << inputData.size() << " samples");
		// resize output :
		etk::Vector<int16_t> output;
		output.resize(inputData.size()*sampleRateOut/sampleRateIn+5000, 0);
		// process in chunk of 256 samples
		int32_t blockSize = 256*nbChan;
		
		Performance perfo;
		audio::algo::speex::Resampler algo;
		algo.init(nbChan, sampleRateIn, sampleRateOut, quality);
		int32_t lastPourcent = -1;
		size_t outputPosition = 0;
		for (int32_t iii=0; iii<inputData.size()/blockSize; ++iii) {
			if (lastPourcent != 100*iii / (inputData.size()/blockSize)) {
				lastPourcent = 100*iii / (inputData.size()/blockSize);
				TEST_INFO("Process : " << iii*blockSize << "/" << int32_t(inputData.size()/blockSize)*blockSize << " " << lastPourcent << "/100");
			} else {
				TEST_VERBOSE("Process : " << iii*blockSize << "/" << int32_t(inputData.size()/blockSize)*blockSize);
			}
			size_t availlableSize = (output.size() - outputPosition) / nbChan;
			perfo.tic();
			algo.process(&output[outputPosition], availlableSize, &inputData[iii*blockSize], blockSize, audio::format_int16);
			if (perf == true) {
				perfo.toc();
				std::this_thread::sleep_for(std::chrono::milliseconds(1));
			}
			outputPosition += availlableSize*nbChan;
		}
		if (perf == true) {
			TEST_INFO("Performance Result: ");
			TEST_INFO("    blockSize=" << blockSize << " sample");
			TEST_INFO("    min=" << perfo.getMinProcessing().count() << " ns");
			TEST_INFO("    max=" << perfo.getMaxProcessing().count() << " ns");
			TEST_INFO("    avg=" << perfo.getTotalTimeProcessing().count()/perfo.getTotalIteration() << " ns");
			
			TEST_INFO("    min=" << (float((perfo.getMinProcessing().count()*sampleRateIn)/blockSize)/1000000000.0)*100.0 << " %");
			TEST_INFO("    max=" << (float((perfo.getMaxProcessing().count()*sampleRateIn)/blockSize)/1000000000.0)*100.0 << " %");
			TEST_INFO("    avg=" << (float(((perfo.getTotalTimeProcessing().count()/perfo.getTotalIteration())*sampleRateIn)/blockSize)/1000000000.0)*100.0 << " %");
		}
		etk::FSNodeWriteAllDataType<int16_t>(outputName, output);
		*/
	}
	
}

