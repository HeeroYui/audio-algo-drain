/** @file
 * @author Edouard DUPIN 
 * @copyright 2011, Edouard DUPIN, all right reserved
 * @license MPL v2.0 (see license file)
 */

#include "debug.hpp"


int32_t audio::algo::drain::getLogId() {
	static int32_t g_val = elog::registerInstance("audio-algo-drain");
	return g_val;
}

