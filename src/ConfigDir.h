/* Read config directories tree 
 *
 * 26/07/2018 - LF - First version
 */
#ifndef CONFIG_DIR_H
#define CONFIG_DIR_H


#include "SortDir.h"

class configDir : virtual public SortDir {
protected :
	virtual bool accept( const char *fch, const char *dir );

public:
	configDir(const char *where) { this->readdircontent( where ); }
};

#endif
