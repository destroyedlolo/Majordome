/* Toile's helpers
 */

#ifndef TOILE_H
#define TOILE_H

#include <cstdint>

#include "../Config.h"

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
		uint16_t width;
		uint16_t height;

		SurfaceGeometry(): x(0), y(0), width(0), height(0){};
	};
}

#endif
