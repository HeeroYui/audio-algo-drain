/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include <etk/types.hpp>
#include <audio/algo/drain/debug.hpp>
#include <audio/algo/drain/BiQuadType.hpp>

static const char* listValues[] = {
	"none",
	"low-pass",
	"high-pass",
	"band-pass",
	"notch",
	"peak",
	"low-shelf",
	"high-shelf"
};
static int32_t listValuesSize = sizeof(listValues)/sizeof(char*);


namespace etk {
	template<> etk::String toString<enum audio::algo::drain::biQuadType>(const enum audio::algo::drain::biQuadType& _variable) {
		return listValues[_variable];
	}
	template <> bool from_string<enum audio::algo::drain::biQuadType>(enum audio::algo::drain::biQuadType& _variableRet, const etk::String& _value) {
		for (int32_t iii=0; iii<listValuesSize; ++iii) {
			if (_value == listValues[iii]) {
				_variableRet = static_cast<enum audio::algo::drain::biQuadType>(iii);
				return true;
			}
		}
		_variableRet = audio::algo::drain::biQuadType_none;
		return false;
	}
}
