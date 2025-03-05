/* Field to display some information
 *
 *	11/01/2025 - First version
 */

#ifndef FIELD_H
#define FIELD_H

#include "Painting.h"
#include "../Handler.h"

class Field : virtual public Object, public Painting, public Handler {
protected:
	virtual void feedState(lua_State *L);

public:
	Field( const std::string &file, std::string &where, std::string &name, lua_State *L );

	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 * -> nameused : is the name already used ?
	 */
	void readConfigDirective( std::string &l, std::string &, bool &nameused );

		/* Executable */
	virtual bool execAsync(lua_State *L);	// Overloading to handle MinMax data feeding
};
#endif
