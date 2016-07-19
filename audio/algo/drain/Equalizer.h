/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <etk/types.h>
#include <ememory/memory.h>
#include <vector>
#include <audio/format.h>
#include <audio/algo/drain/BiQuadType.h>

namespace audio {
	namespace algo {
		namespace drain {
			class EqualizerPrivate;
			class Equalizer {
				public:
					/**
					 * @brief Constructor
					 */
					Equalizer();
					/**
					 * @brief Destructor
					 */
					virtual ~Equalizer();
				public:
					/**
					 * @brief Reset all history of the Algo.
					 */
					void reset();
					/**
					 * @brief Initialize the Algorithm
					 * @param[in] _sampleRate Sample rate of the stream.
					 * @param[in] _nbChannel Number of channel in the stream.
					 * @param[in] _format Input data format.
					 */
					virtual void init(float _sampleRate=48000, int8_t _nbChannel=2, enum audio::format _format=audio::format_float);
					/**
					 * @brief Get list of format suported in input.
					 * @return list of supported format
					 */
					virtual std::vector<enum audio::format> getSupportedFormat();
					/**
					 * @brief Get list of algorithm format suported. No format convertion.
					 * @return list of supported format
					 */
					virtual std::vector<enum audio::format> getNativeSupportedFormat();
					/**
					 * @brief Main input algo process.
					 * @param[in,out] _output Output data.
					 * @param[in] _input Input data.
					 * @param[in] _nbChunk Number of chunk in the input buffer.
					 */
					virtual void process(void* _output, const void* _input, size_t _nbChunk);
				public:
					/**
					 * @brief add a biquad with his value.
					 */
					bool addBiquad(double _a0, double _a1, double _a2, double _b0, double _b1);
					bool addBiquad(int32_t _idChannel, double _a0, double _a1, double _a2, double _b0, double _b1);
					/**
					 * @brief add a bi-quad value and type
					 * @param[in] _type Type of biquad.
					 * @param[in] _frequencyCut Cut Frequency. [0..sampleRate/2]
					 * @param[in] _qualityFactor Q factor of quality (good value of 0.707 ==> permit to not ower gain) limit [0.01 .. 10]
					 * @param[in] _gain Gain to apply (for notch, peak, lowShelf and highShelf) limit : -30, +30
					 * @param[in] _sampleRate Sample rate of the signal
					 */
					bool addBiquad(audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain);
					bool addBiquad(int32_t _idChannel, audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain);
				public:
					// for debug & tools only
					std::vector<std::pair<float,float> > calculateTheory();
				protected:
					ememory::SharedPtr<EqualizerPrivate> m_private; //!< private data (abstract the type of the data flow).
			};
		}
	}
}

