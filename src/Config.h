/* Configuration handling
 *
 * 26/07/2018 - LF - First version
 * 27/07/2018 - LF - handle all Configuration aspects
 */
#ifndef CONFIG_H
#define CONFIG_H

#include <map>

#include <lua.h>	/* Lua's state needed */

#include "SortDir.h"
#include "MQTTTopic.h"

class Config : virtual public SortDir {
protected :
	virtual bool accept( const char *, string & );

public:
	typedef std::map<std::string, MQTTTopic> TopicElements;
	TopicElements TopicsList;

	Config(const char *where, lua_State *L);

	/* Check if data overlaps
	 *	Notez-bien : in case of clash, the program exits
	 */
	void SanityChecks( void );
};

#endif
