/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */
#pragma once

#include <ememory/memory.hpp>

namespace audio {
	namespace algo {
		namespace drain {
			enum biQuadType {
				biQuadType_none, //!< no filter (pass threw...)
				biQuadType_lowPass, //!< low pass filter
				biQuadType_highPass, //!< High pass filter
				biQuadType_bandPass, //!< band pass filter
				biQuadType_notch, //!< Notch Filter
				biQuadType_peak, //!< Peaking band EQ filter
				biQuadType_lowShelf, //!< Low shelf filter
				biQuadType_highShelf, //!< High shelf filter
			};
		}
	}
}

