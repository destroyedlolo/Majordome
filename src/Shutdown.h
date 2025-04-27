/* Task to be launched at Majordome's shutdown
 *
 * 9/12/2024 - LF - First version
 */

#ifndef SHUTDOWN_H 
#define SHUTDOWN_H

#include "LuaExec.h"
#include "ObjCollection.h"

class Shutdown : public LuaExec {
public:
	/* Constructor from a file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	Shutdown( const std::string &file, std::string &where, lua_State *L );

	void exec( void );

	/* Create Lua's object */
	static void initLuaInterface( lua_State *L );
};

typedef ObjCollection<Shutdown *> ShutdownCollection;
#endif
