/* LCD Renderer
 * 	Embodies a low-level physical LCD textual device.
 *
 * 	21/06/2025 - First version
 */

#ifndef LCD_H
#define LCD_H

#include "Renderer.h"

class LCD : virtual public Renderer, virtual public LuaExec {
	uint16_t bus_number;
	uint8_t address;
	bool twolines;
	bool y11;

	uint32_t w,h;	// Screen size
	useconds_t clock_pulse;
	useconds_t clock_process;

protected:
	/* Read directives.
	 * These directives may apply to all derivates.
	 * Facing unknown directive, LuaExec's is called as well.
	 *
	 * -> l : directive line to parse
	 */
	void readConfigDirective( std::string &l );

public:
	/* constructor from file
	 * -> file : file to load
	 * -> where : file's directory
	 * <- name : this object's name
	 * -> L : Lua's state
	 */
	LCD( const std::string &file, std::string &where, lua_State *L );

	bool exec(); 		// Run the Lua code to validate the renderer

	virtual std::string getTri(){ return "LCD_"; }
};

#endif
