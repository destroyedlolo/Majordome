/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 */

#ifndef SUBCONFIGDIR_H
#define SUBCONFIGDIR_H

#include "SortDir.h"

#include <lua.hpp>	/* Lua's state needed */
#include <cstdint>

class Config;

class SubConfigDir : public SortDir {
public :
	struct extweight {
		const char *const ext;
		uint8_t weight;
	};

protected :
	virtual bool accept( const char *, std::string & );

public:
	SubConfigDir(Config &, std::string &, lua_State *);

	virtual void sort( void );
};

#endif
