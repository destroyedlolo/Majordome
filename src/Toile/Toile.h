/* Toile's helpers
 */

#ifndef TOILE_H
#define TOILE_H

#include <cstdint>

// Can't be included as creating rouding dependancies
// #include "../Config.h"

#include <string>
#include <lua.hpp>	/* Lua's state needed */
class Config;

namespace Toile {
	uint8_t objectweight( const char *ext );
	bool readConfigToile(Config &cfg, std::string &completpath, std::string &where, const char *ext, lua_State *L);

	bool execRenderers();		// Exec Renderers own code
	void RefreshRenderers();	// Exec Renderers' Decoration

	struct Coordinates {
		uint32_t x;
		uint32_t y;

		Coordinates(): x(0), y(0){};
	};

	struct SurfaceGeometry {
		uint32_t x;
		uint32_t y;
		uint32_t w;
		uint32_t h;

		SurfaceGeometry(): x(0), y(0), w(0), h(0){};
	};
}

#endif
