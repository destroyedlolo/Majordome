/* Add a data to table
 */
#ifndef FEED_H
#define FEED_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"

class pgSQL;

class Feed : public Handler {
	mpgConnection *db;

	virtual void readConfigDirective( std::string &l, std::string &name, bool &nameused );
	virtual void feedState(lua_State *L);

protected:
	virtual const char *getNameC(){ return(this->Object::getNameC()); };

public:
	Feed(const std::string &fch, std::string &where, std::string &name, lua_State *L);
};

typedef ObjCollection<Feed *> FeedCollection;
#endif
