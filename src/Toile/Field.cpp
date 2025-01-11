/* Field to display some information
 *
 *	11/01/2025 - First version
 */

#include "Field.h"

Field::Field( const std::string &fch, std::string &where, std::string &name, lua_State *L ) : Painting(fch, where, name){
	if(verbose)
		SelLog->Log('L', "\t'%s'", fch.c_str());
}
