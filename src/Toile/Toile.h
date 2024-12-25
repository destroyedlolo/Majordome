/* Toile's helpers
 */

#ifndef TOILE_H
#define TOILE_H

#include <cstdint>

#include "../Config.h"

namespace Toile {
	uint8_t objectweight( const char *ext );
	bool readConfigToile(Config &cfg, std::string &completpath, std::string &where, const char *ext, lua_State *L);

	bool execRenderers();
	void RefreshRenderers();
}

#endif
