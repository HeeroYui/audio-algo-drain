/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */

#include <audio/algo/drain/Equalizer.hpp>
#include <audio/algo/drain/debug.hpp>
#include <audio/algo/drain/BiQuad.hpp>
#include <audio/types.hpp>


// see http://www.musicdsp.org/files/Audio-EQ-Cookbook.txt
// see http://www.earlevel.com/main/2013/10/13/biquad-calculator-v2/

namespace audio {
	namespace algo {
		namespace drain {
			class EqualizerPrivate {
				protected:
					float m_sampleRate;
					int8_t m_nbChannel;
				public:
					/**
					 * @brief Constructor
					 */
					EqualizerPrivate() :
					  m_sampleRate(48000),
					  m_nbChannel(2) {
						
					}
					/**
					 * @brief Destructor
					 */
					virtual ~EqualizerPrivate() {
						
					}
				public:
					/**
					 * @brief Reset all history of the Algo.
					 */
					virtual void reset() = 0;
					/**
					 * @brief Initialize the Algorithm
					 * @param[in] _sampleRate Sample rate of the stream.
					 * @param[in] _nbChannel Number of channel in the stream.
					 */
					virtual void init(float _sampleRate=48000, int8_t _nbChannel=2) {
						m_sampleRate = _sampleRate;
						m_nbChannel = _nbChannel;
					};
					/**
					 * @brief Main input algo process.
					 * @param[in,out] _output Output data.
					 * @param[in] _input Input data.
					 * @param[in] _nbChunk Number of chunk in the input buffer.
					 */
					virtual void process(void* _output, const void* _input, size_t _nbChunk) = 0;
				public:
					/**
					 * @brief add a biquad with his value.
					 */
					virtual bool addBiquad(double _a0, double _a1, double _a2, double _b0, double _b1) = 0;
					virtual bool addBiquad(int32_t _idChannel, double _a0, double _a1, double _a2, double _b0, double _b1) = 0;
					/**
					 * @brief add a bi-quad value and type
					 * @param[in] _type Type of biquad.
					 * @param[in] _frequencyCut Cut Frequency. [0..sampleRate/2]
					 * @param[in] _qualityFactor Q factor of quality (good value of 0.707 ==> permit to not ower gain) limit [0.01 .. 10]
					 * @param[in] _gain Gain to apply (for notch, peak, lowShelf and highShelf) limit : -30, +30
					 * @param[in] _sampleRate Sample rate of the signal
					 */
					virtual bool addBiquad(audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain) = 0;
					virtual bool addBiquad(int32_t _idChannel, audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain) = 0;
				public:
					// for debug & tools only
					virtual std::vector<std::pair<float,float> > calculateTheory() = 0;
			};
			template<typename TYPE> class EqualizerPrivateType : public audio::algo::drain::EqualizerPrivate {
				protected:
					std::vector<std::vector<audio::algo::drain::BiQuad<TYPE> > > m_biquads;
				public:
					/**
					 * @brief Constructor
					 */
					EqualizerPrivateType() {
						
					}
					/**
					 * @brief Destructor
					 */
					virtual ~EqualizerPrivateType() {
						
					}
					virtual void reset() {
						for (size_t jjj=0; jjj<m_biquads.size(); ++jjj) {
							for (size_t iii=0; iii<m_biquads[jjj].size(); ++iii) {
								m_biquads[jjj][iii].reset();
							}
						}
					}
					virtual void init(float _sampleRate=48000, int8_t _nbChannel=2) {
						audio::algo::drain::EqualizerPrivate::init(_sampleRate, _nbChannel);
						m_biquads.clear();
						m_biquads.resize(_nbChannel);
					}
					virtual void process(void* _output, const void* _input, size_t _nbChunk) {
						for (size_t jjj=0; jjj<m_nbChannel; ++jjj) {
							// get pointer on data:
							const TYPE* input = reinterpret_cast<const TYPE*>(_input);
							TYPE* output = reinterpret_cast<TYPE*>(_output);
							// move to sample offset:
							input += jjj;
							output += jjj;
							for (size_t iii=0; iii<m_biquads[jjj].size(); ++iii) {
								m_biquads[jjj][iii].process(input, output, _nbChunk, m_nbChannel, m_nbChannel);
							}
						}
					}
					virtual bool addBiquad(double _a0, double _a1, double _a2, double _b0, double _b1) {
						audio::algo::drain::BiQuad<TYPE> bq;
						bq.setBiquadCoef(_a0, _a1, _a2, _b0, _b1);
						// add this bequad for every Channel:
						for (size_t iii=0; iii<m_biquads.size(); ++iii) {
							m_biquads[iii].push_back(bq);
						}
						return true;
					}
					virtual bool addBiquad(audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain) {
						audio::algo::drain::BiQuad<TYPE> bq;
						bq.setBiquad(_type, _frequencyCut, _qualityFactor, _gain, m_sampleRate);
						// add this bequad for every Channel:
						for (size_t iii=0; iii<m_biquads.size(); ++iii) {
							m_biquads[iii].push_back(bq);
						}
						return true;
					}
					virtual bool addBiquad(int32_t _idChannel, double _a0, double _a1, double _a2, double _b0, double _b1) {
						audio::algo::drain::BiQuad<TYPE> bq;
						bq.setBiquadCoef(_a0, _a1, _a2, _b0, _b1);
						if (_idChannel<m_biquads.size()) {
							m_biquads[_idChannel].push_back(bq);
						}
						return true;
					}
					virtual bool addBiquad(int32_t _idChannel, audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain) {
						audio::algo::drain::BiQuad<TYPE> bq;
						bq.setBiquad(_type, _frequencyCut, _qualityFactor, _gain, m_sampleRate);
						if (_idChannel<m_biquads.size()) {
							m_biquads[_idChannel].push_back(bq);
						}
						return true;
					}
					virtual std::vector<std::pair<float,float> > calculateTheory() {
						std::vector<std::pair<float,float> > out;
						for (size_t iii=0; iii<m_biquads[0].size(); ++iii) {
							if (iii == 0) {
								out = m_biquads[0][iii].calculateTheory(m_sampleRate);
							} else {
								std::vector<std::pair<float,float> > tmp = m_biquads[0][iii].calculateTheory(m_sampleRate);
								for (size_t jjj=0; jjj< out.size(); ++jjj) {
									out[jjj].second += tmp[jjj].second;
								}
							}
						}
						return out;
					}
			};
		}
	}
}

audio::algo::drain::Equalizer::Equalizer() {
	
}

audio::algo::drain::Equalizer::~Equalizer() {
	
}

void audio::algo::drain::Equalizer::init(float _sampleRate, int8_t _nbChannel, enum audio::format _format) {
	switch (_format) {
		default:
			AA_DRAIN_CRITICAL("Request format for equalizer that not exist ... : " << _format);
			break;
		case audio::format_double:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::double_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_float:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::float_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int8:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int8_8_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int8_on_int16:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int8_16_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int16:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int16_16_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int16_on_int32:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int16_32_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int24_on_int32:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int24_32_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int32:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int32_32_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int32_on_int64:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int32_64_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
		case audio::format_int64:
			{
				m_private = ememory::makeShared<EqualizerPrivateType<audio::int32_64_t> >();
				if (m_private == nullptr) {
					AA_DRAIN_ERROR("can not allocate private data...");
					return;
				}
				m_private->init(_sampleRate, _nbChannel);
			}
			break;
	}
}
void audio::algo::drain::Equalizer::reset() {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return;
	}
	m_private->reset();
}

std::vector<enum audio::format> audio::algo::drain::Equalizer::getSupportedFormat() {
	std::vector<enum audio::format> out = audio::algo::drain::Equalizer::getNativeSupportedFormat();
	
	return out;
}

std::vector<enum audio::format> audio::algo::drain::Equalizer::getNativeSupportedFormat() {
	std::vector<enum audio::format> out;
	out.push_back(audio::format_float);
	return out;
}

void audio::algo::drain::Equalizer::process(void* _output, const void* _input, size_t _nbChunk) {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return;
	}
	m_private->process(_output, _input, _nbChunk);
}

bool audio::algo::drain::Equalizer::addBiquad(double _a0, double _a1, double _a2, double _b0, double _b1) {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return false;
	}
	return m_private->addBiquad(_a0, _a1, _a2, _b0, _b1);
}
bool audio::algo::drain::Equalizer::addBiquad(int32_t _idChannel, double _a0, double _a1, double _a2, double _b0, double _b1) {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return false;
	}
	return m_private->addBiquad(_idChannel, _a0, _a1, _a2, _b0, _b1);
}

bool audio::algo::drain::Equalizer::addBiquad(audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain) {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return false;
	}
	return m_private->addBiquad(_type, _frequencyCut, _qualityFactor, _gain);
}
bool audio::algo::drain::Equalizer::addBiquad(int32_t _idChannel, audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain) {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return false;
	}
	return m_private->addBiquad(_idChannel, _type, _frequencyCut, _qualityFactor, _gain);
}

std::vector<std::pair<float,float> > audio::algo::drain::Equalizer::calculateTheory() {
	if (m_private == nullptr) {
		AA_DRAIN_ERROR("Equalizer does not init ...");
		return std::vector<std::pair<float,float> >();
	}
	return m_private->calculateTheory();
}





