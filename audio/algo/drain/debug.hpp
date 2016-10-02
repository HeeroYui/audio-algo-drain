/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license APACHE v2.0 (see license file)
 */
#pragma once

#include <elog/log.hpp>

namespace audio {
	namespace algo {
		namespace drain {
			int32_t getLogId();
		}
	}
}

#define AA_DRAIN_BASE(info,data) ELOG_BASE(audio::algo::drain::getLogId(),info,data)

#define AA_DRAIN_PRINT(data)         AA_DRAIN_BASE(-1, data)
#define AA_DRAIN_CRITICAL(data)      AA_DRAIN_BASE(1, data)
#define AA_DRAIN_ERROR(data)         AA_DRAIN_BASE(2, data)
#define AA_DRAIN_WARNING(data)       AA_DRAIN_BASE(3, data)
#ifdef DEBUG
	#define AA_DRAIN_INFO(data)          AA_DRAIN_BASE(4, data)
	#define AA_DRAIN_DEBUG(data)         AA_DRAIN_BASE(5, data)
	#define AA_DRAIN_VERBOSE(data)       AA_DRAIN_BASE(6, data)
	#define AA_DRAIN_TODO(data)          AA_DRAIN_BASE(4, "TODO : " << data)
#else
	#define AA_DRAIN_INFO(data)          do { } while(false)
	#define AA_DRAIN_DEBUG(data)         do { } while(false)
	#define AA_DRAIN_VERBOSE(data)       do { } while(false)
	#define AA_DRAIN_TODO(data)          do { } while(false)
#endif

#define AA_DRAIN_ASSERT(cond,data) \
	do { \
		if (!(cond)) { \
			AA_DRAIN_CRITICAL(data); \
			assert(!#cond); \
		} \
	} while (0)

