/* Decoration object
 * 	Task to be called when an object needs to be refreshed
 *
 * 	19/12/2024 - First version
 */

#ifndef DECORATION_H
#define DECORATION_H

class Decoration;	// Avoid nested includes

#include "ToileObject.h"
#include "../Helpers.h"
#include "../Object.h"
#include "../LuaExec.h"

#include <Selene/SelGenericSurface.h>

class Decoration : public ToileObject, virtual public LuaExec {
public:
	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Decoration( const std::string &file, std::string &where, lua_State *L );
	
	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 * -> nameused : is the name already used ?
	 */
	bool readConfigDirective( std::string &l );

	/* Run the Lua code to build the renderer */
	void exec(struct SelGenericSurface *);

	virtual std::string getTri(){ return "DEC_"; }
};

typedef ObjCollection<Decoration *> DecorationCollection;
#endif
