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
		uint16_t x;
		uint16_t y;

		Coordinates(): x(0), y(0){};
	};

	struct SurfaceGeometry {
		uint16_t x;
		uint16_t y;
		uint16_t w;
		uint16_t h;

		SurfaceGeometry(): x(0), y(0), w(0), h(0){};
	};
}

#endif
