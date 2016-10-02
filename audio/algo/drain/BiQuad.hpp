/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <ememory/memory.hpp>
#include <etk/types.hpp>
#include <audio/algo/drain/BiQuadType.hpp>
#include <cmath>
#ifndef M_LN10
	#define M_LN10 2.30258509299404568402
#endif
#ifndef M_SQRT2
	#define M_SQRT2 1.41421356237309504880
#endif

namespace audio {
	namespace algo {
		namespace drain {
			template<typename TYPE> class BiQuad {
				public:
					BiQuad() {
						reset();
						// reset coefficients
						m_a[0] = 1.0;
						m_a[1] = 0.0;
						m_a[2] = 0.0;
						m_b[0] = 0.0;
						m_b[1] = 0.0;
					}
				protected:
					TYPE m_x[2]; //!< X history
					TYPE m_y[2]; //!< Y histiry
					TYPE m_a[3]; //!< A bi-Quad coef
					TYPE m_b[2]; //!< B bi-Quad coef
				public:
					/**
					 * @brief Set the bi-quad value and type
					 * @param[in] _type Type of biquad.
					 * @param[in] _frequencyCut Cut Frequency. [0..sampleRate/2]
					 * @param[in] _qualityFactor Q factor of quality (good value of 0.707 ==> permit to not ower gain) limit [0.01 .. 10]
					 * @param[in] _gain Gain to apply (for notch, peak, lowShelf and highShelf) limit : -30, +30
					 * @param[in] _sampleRate Sample rate of the signal
					 */
					void setBiquad(enum audio::algo::drain::biQuadType _type, double _frequencyCut, double _qualityFactor, double _gain, float _sampleRate) {
						reset();
						if (_sampleRate < 1) {
							m_a[0] = 1.0;
							m_a[1] = 0.0;
							m_a[2] = 0.0;
							m_b[0] = 0.0;
							m_b[1] = 0.0;
							return;
						}
						if (_frequencyCut > _sampleRate/2) {
							_frequencyCut = _sampleRate/2;
						} else if (_frequencyCut < 0) {
							_frequencyCut = 0;
						}
						if (_qualityFactor < 0.01) {
							_qualityFactor = 0.01;
						}
						double norm;
						double V = std::pow(10.0, std::abs(_gain) / 20.0);
						double K = std::tan(M_PI * _frequencyCut / _sampleRate);
						switch (_type) {
							case biQuadType_none:
								m_a[0] = 1.0;
								m_a[1] = 0.0;
								m_a[2] = 0.0;
								m_b[0] = 0.0;
								m_b[1] = 0.0;
								break;
							case biQuadType_lowPass:
								norm = 1.0 / (1.0 + K / _qualityFactor + K * K);
								m_a[0] = K * K * norm;
								m_a[1] = m_a[0] * 2.0;
								m_a[2] = m_a[0];
								m_b[0] = 2.0 * (K * K - 1.0) * norm;
								m_b[1] = (1.0 - K / _qualityFactor + K * K) * norm;
								break;
							case biQuadType_highPass:
								norm = 1.0 / (1.0 + K / _qualityFactor + K * K);
								m_a[0] = 1.0 * norm;
								m_a[1] = m_a[0] * -2.0;
								m_a[2] = m_a[0];
								m_b[0] = 2.0 * (K * K - 1.0) * norm;
								m_b[1] = (1.0 - K / _qualityFactor + K * K) * norm;
								break;
							case biQuadType_bandPass:
								norm = 1.0 / (1.0 + K / _qualityFactor + K * K);
								m_a[0] = K / _qualityFactor * norm;
								m_a[1] = 0.0;
								m_a[2] = m_a[0] * -1.0;
								m_b[0] = 2.0 * (K * K - 1.0) * norm;
								m_b[1] = (1.0 - K / _qualityFactor + K * K) * norm;
								break;
							case biQuadType_notch:
								norm = 1.0 / (1.0 + K / _qualityFactor + K * K);
								m_a[0] = (1.0 + K * K) * norm;
								m_a[1] = 2.0 * (K * K - 1.0) * norm;
								m_a[2] = m_a[0];
								m_b[0] = m_a[1];
								m_b[1] = (1.0 - K / _qualityFactor + K * K) * norm;
								break;
							case biQuadType_peak:
								if (_gain >= 0.0) {
									norm = 1.0 / (1.0 + 1.0/_qualityFactor * K + K * K);
									m_a[0] = (1.0 + V/_qualityFactor * K + K * K) * norm;
									m_a[1] = 2.0 * (K * K - 1.0) * norm;
									m_a[2] = (1.0 - V/_qualityFactor * K + K * K) * norm;
									m_b[0] = m_a[1];
									m_b[1] = (1.0 - 1.0/_qualityFactor * K + K * K) * norm;
								} else {
									norm = 1.0 / (1.0 + V/_qualityFactor * K + K * K);
									m_a[0] = (1.0 + 1.0/_qualityFactor * K + K * K) * norm;
									m_a[1] = 2.0 * (K * K - 1.0) * norm;
									m_a[2] = (1.0 - 1.0/_qualityFactor * K + K * K) * norm;
									m_b[0] = m_a[1];
									m_b[1] = (1.0 - V/_qualityFactor * K + K * K) * norm;
								}
								break;
							case biQuadType_lowShelf:
								if (_gain >= 0) {
									norm = 1.0 / (1.0 + M_SQRT2 * K + K * K);
									m_a[0] = (1.0 + std::sqrt(2.0*V) * K + V * K * K) * norm;
									m_a[1] = 2.0 * (V * K * K - 1.0) * norm;
									m_a[2] = (1.0 - std::sqrt(2.0*V) * K + V * K * K) * norm;
									m_b[0] = 2.0 * (K * K - 1.0) * norm;
									m_b[1] = (1.0 - M_SQRT2 * K + K * K) * norm;
								} else {
									norm = 1.0 / (1.0 + std::sqrt(2.0*V) * K + V * K * K);
									m_a[0] = (1.0 + M_SQRT2 * K + K * K) * norm;
									m_a[1] = 2.0 * (K * K - 1.0) * norm;
									m_a[2] = (1.0 - M_SQRT2 * K + K * K) * norm;
									m_b[0] = 2.0 * (V * K * K - 1.0) * norm;
									m_b[1] = (1.0 - std::sqrt(2.0*V) * K + V * K * K) * norm;
								}
								break;
							case biQuadType_highShelf:
								if (_gain >= 0) {
									norm = 1.0 / (1.0 + M_SQRT2 * K + K * K);
									m_a[0] = (V + std::sqrt(2.0*V) * K + K * K) * norm;
									m_a[1] = 2.0 * (K * K - V) * norm;
									m_a[2] = (V - std::sqrt(2.0*V) * K + K * K) * norm;
									m_b[0] = 2.0 * (K * K - 1.0) * norm;
									m_b[1] = (1.0 - M_SQRT2 * K + K * K) * norm;
								} else {
									norm = 1.0 / (V + std::sqrt(2.0*V) * K + K * K);
									m_a[0] = (1.0 + M_SQRT2 * K + K * K) * norm;
									m_a[1] = 2.0 * (K * K - 1.0) * norm;
									m_a[2] = (1.0 - M_SQRT2 * K + K * K) * norm;
									m_b[0] = 2.0 * (K * K - V) * norm;
									m_b[1] = (V - std::sqrt(2.0*V) * K + K * K) * norm;
								}
								break;
						}
					}
					/**
					 * @brief Set direct Coefficients
					 */
					void setBiquadCoef(TYPE _a0, TYPE _a1, TYPE _a2, TYPE _b0, TYPE _b1) {
						m_a[0] = _a0;
						m_a[1] = _a1;
						m_a[2] = _a2;
						m_b[0] = _b0;
						m_b[1] = _b1;
						reset();
					}
					/**
					 * @brief Get direct Coefficients
					 */
					void getBiquadCoef(TYPE& _a0, TYPE& _a1, TYPE& _a2, TYPE& _b0, TYPE& _b1) {
						_a0 = m_a[0];
						_a1 = m_a[1];
						_a2 = m_a[2];
						_b0 = m_b[0];
						_b1 = m_b[1];
					}
					/**
					 * @brief Get direct Coefficients
					 */
					std::vector<TYPE> getCoef() {
						std::vector<TYPE> out;
						out.push_back(m_a[0]);
						out.push_back(m_a[1]);
						out.push_back(m_a[2]);
						out.push_back(m_b[0]);
						out.push_back(m_b[1]);
						return out;
					}
					/**
					 * @brief Reset bequad filter (only history not value).
					 */
					void reset() {
						m_x[0] = 0;
						m_y[1] = 0;
						m_x[0] = 0;
						m_y[1] = 0;
					}
				protected:
					/**
					 * @brief process single sample in float.
					 * @param[in] _sample Sample to process
					 * @return updataed value
					 */
					TYPE process(TYPE _sample) {
						TYPE result;
						// compute
						result =   m_a[0] * _sample
						         + m_a[1] * m_x[0]
						         + m_a[2] * m_x[1]
						         - m_b[0] * m_y[0]
						         - m_b[1] * m_y[1];
						//update history of X
						m_x[1] = m_x[0];
						m_x[0] = _sample;
						//update history of Y
						m_y[1] = m_y[0];
						m_y[0] = result;
						return result;
					}
				public:
					/**
					 * @brief Porcess function.
					 * param[in] _input Pointer on the input data.
					 * param[in,out] _output Poirter on the output data (can be the same as input (inplace availlable).
					 * param[in] _nbChunk Number of qample to process.
					 * param[in] _inputOffset Offset to add when read input data.
					 * param[in] _outputOffset Offset to add when write output data.
					 */
					void process(const TYPE* _input,
					             TYPE* _output,
					             size_t _nbChunk,
					             int32_t _inputOffset,
					             int32_t _outputOffset) {
						for (size_t iii=0; iii<_nbChunk; ++iii) {
							// process in float the biquad.
							*_output = process(*_input);
							// move to the sample on the same channel.
							_input += _inputOffset;
							_output += _outputOffset;
						}
					}
					/**
					 * @brief calculate respond of the filter:
					 * @param[in] _sampleRate input qample rate
					 * @retrun list of frequency/power in dB
					 */
					std::vector<std::pair<float,float> > calculateTheory(double _sampleRate){
						std::vector<std::pair<float,float> > out;
						double norm;
						bool buildLinear = true;
						size_t len = 512;
						for (size_t iii=0; iii < len; iii++) {
							double w;
							if (buildLinear == true) {
								// 0 to pi, linear scale
								w = iii / (len - 1.0) * M_PI;
							} else {
								// 0.001 to 1, times pi, log scale
								w = std::exp(std::log(1.0 / 0.001) * iii / (len - 1.0)) * 0.001 * M_PI;
							}
							double freq = iii / (len - 1.0) * _sampleRate / 2.0;
							double phi = std::pow(std::sin(w/2.0), 2.0);
							double y =   std::log(   std::pow((m_a[0]+m_a[1]+m_a[2]).getDouble(), 2.0)
							                       - 4.0*((m_a[0]*m_a[1]).getDouble() + 4.0*(m_a[0]*m_a[2]).getDouble() + (m_a[1]*m_a[2]).getDouble())*phi
							                       + 16.0*(m_a[0]*m_a[2]).getDouble()*phi*phi)
							           - std::log(   std::pow(1.0+(m_b[0]+m_b[1]).getDouble(), 2.0)
							                       - 4.0*((m_b[0]).getDouble() + 4.0*(m_b[1]).getDouble() + (m_b[0]*m_b[1]).getDouble())*phi
							                       + 16.0*m_b[1].getDouble()*phi*phi);
							y = y * 10.0 / M_LN10;
							if (y <= -200) {
								y = -200.0;
							}
							//APPL_DEBUG("theory = " << freq << " power=" << y);
							out.push_back(std::make_pair<float,float>(freq, y));
						}
						return out;
					}
			};
		}
	}
}

