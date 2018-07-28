/* Package configuration handling
 *
 * 27/07/2018 - LF - First version
 */

#ifndef SUBCONFIGDIR_H
#define SUBCONFIGDIR_H

#include "Config.h"
#include "SortDir.h"

class SubConfigDir : virtual public SortDir {
protected :
	virtual bool accept( const char *, string & );

public:
	SubConfigDir(Config &, string &, lua_State *);
};

#endif
