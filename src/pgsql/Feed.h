/* Add a data to table
 */
#ifndef FEED_H
#define FEED_H

#include "mpgConnection.h"
#include "../Object.h"
#include "../ObjCollection.h"

class pgSQL;

class Feed : public mpgConnection, public Object {
public:
	Feed(const std::string &fch, std::string &where, std::string &name);
};

typedef ObjCollection<Feed *> FeedCollection;
#endif
