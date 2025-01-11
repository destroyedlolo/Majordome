/* Field to display some information
 *
 *	11/01/2025 - First version
 */

#ifndef FIELD_H
#define FIELD_H

#include "Painting.h"

class Field : public Painting {
public:
	Field( const std::string &file, std::string &where, std::string &name, lua_State *L );

};

#endif
