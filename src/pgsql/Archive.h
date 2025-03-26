/* Archive data
 */
#ifdef ARCHIVE_H
#define ARCHIVE_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../Handler.h"
#include "../ObjCollection.h"

class Archive : virtual public mpgConnection, virtual public Handler {
protected:
	void readConfigDirective( std::string &l, std::string &name, bool &nameused );

public:
	Archive(const std::string &fch, std::string &where, std::string &name, lua_State *L);

	/* Accessors */
	const char *getTableName(void);
	virtual const char *getNameC(){ return(this->Object::getNameC()); };
	bool isQuiet(){ return this->Object::isQuiet(); };
}

typedef ObjCollection<Archive *> ArchiveCollection;
#endif
