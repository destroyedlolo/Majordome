/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 */

#ifndef SUBCONFIGDIR_H
#define SUBCONFIGDIR_H

#include "SortDir.h"

class SubConfigDir : virtual public SortDir {
protected :
	virtual bool accept( const char *fch, const char *dir );

public:
	SubConfigDir(const char *where, lua_State *L);
};

#endif
