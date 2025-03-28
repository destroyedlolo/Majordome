#include "Feed.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

Feed::Feed(const std::string &fch, std::string &where, std::string &name, lua_State *L): Object(fch, where, name), Handler(fch, where, name) {
	/*
	 * Reading file's content
	 */

	std::stringstream buffer;
	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit );
	try {
		std::ifstream file(fch);
		std::streampos pos;

		bool nameused = false;	// if so, the name can't be changed anymore

		/*
		 * Reading header (Majordome's commands)
		 */

		do {
			std::string l;
			pos = file.tellg();

			std::getline( file, l);
			if( l.compare(0, 2, "--") ){	// End of comments
				file.seekg( pos );
				break;
			}

			this->readConfigDirective(l, name, nameused);
		} while(true);


		/*
		 * Reading the remaining of the script and keep it as 
		 * an Lua's script
		 */
		buffer << file.rdbuf();
		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	if( !this->LoadFunc( L, buffer, this->name.c_str() ))
		exit(EXIT_FAILURE);
}

void Feed::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if(!!(arg = striKWcmp( l, "-->> listen=" ))){
		TopicCollection::iterator topic;
		if( (topic = config.TopicsList.find(arg)) != config.TopicsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded to topic '%s'", arg.c_str());
			topic->second->addHandler( dynamic_cast<Handler *>(this) );
//			nameused = true;
		} else {
			SelLog->Log('F', "\t\tTopic '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if( !!(arg = striKWcmp( l, "-->> when=" ))){
		TimerCollection::iterator timer;
		if( (timer = config.TimersList.find(arg)) != config.TimersList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tAdded to timer '%s'", arg.c_str());
			timer->second->addHandler( dynamic_cast<Handler *>(this) );
//			nameused = true;
		} else {
			SelLog->Log('F', "\t\ttimer '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> table=" ))){
		this->TableName = arg;
			if(verbose)
				SelLog->Log('C', "\t\tTable : %s", arg.c_str());
#if 0/* For the moment, the value can be only numeric */
	} else if(l == "-->> Numerical"){
		this->numerical = true;
			if(verbose)
				SelLog->Log('C', "\t\tNumerical");
#endif
	} else if(!!(arg = striKWcmp( l, "-->> Database=" ))){
		pgSQLCollection::iterator db;
		if( (db = config.pgSQLsList.find(arg)) != config.pgSQLsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tDatabase : %s", arg.c_str());
			this->db = db->second;
//			nameused = true;
		} else {
			SelLog->Log('F', "\t\tDatabase '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else 
		this->LuaExec::readConfigDirective(l, name, nameused);
}

void Feed::feedState( lua_State *L ){
	class Feed **feed= (class Feed **)lua_newuserdata(L, sizeof(class Feed *));
	assert(feed);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_FEED" );

	*feed = this;
	luaL_getmetatable(L, "MajordomeFeed");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

const char *Feed::getTableName(void){
	if(!!this->TableName)
		return(this->TableName.c_str());
	else
		return(this->getNameC());
}

bool Feed::execAsync(lua_State *L){
	/* To prevent SQL injection
	 * https://www.postgresql.org/docs/current/libpq-exec.html#LIBPQ-EXEC-ESCAPE-STRING
	insert into test values (now(), 10);
	*/

	LuaExec::boolRetCode rc;
	lua_Number val;

	bool r = this->LuaExec::execSync(L, &rc, &val);
	if( rc != LuaExec::boolRetCode::RCfalse ){	// data not rejected
		if(isnan(val)){	// data unchanged
			lua_getglobal(L, "MAJORDOME_PAYLOAD");
			if(lua_isnumber(L, -1))
				val = lua_tonumber(L, -1);
			else {
				SelLog->Log('E', "['%s'] can't find MAJORDOME_PAYLOAD as number", this->getNameC());
				lua_close(L);
				return r;
			}
		}
		
		if(debug && !this->isQuiet())
			SelLog->Log('T', "['%s'] accepting %.0f", this->getNameC(), val);

		/* Build SQL request */
		if(!this->connect()){
			lua_close(L);
			return false;
		}

		char *t;
		std::string cmd("INSERT INTO ");
		cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
		PQfreemem(t);

		cmd += " VALUES ( now(), ",
		cmd += std::to_string(val);
		cmd += " )";

		if(!this->doSQL(cmd.c_str()))
			SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
	
		this->disconnect();
		lua_close(L);
	} else
		SelLog->Log('D', "['%s'] Data rejected", this->getNameC());

	return r;
}

	/*****
	 * Lua exposed functions
	 *****/

static class Feed *checkMajordomeFeed(lua_State *L){
	class Feed **r = (class Feed **)SelLua->testudata(L, 1, "MajordomeFeed");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeFeed' expected");
	return *r;
}

static int lfd_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class Feed *f = config.FeedsList.at( name );
		class Feed **feed = (class Feed **)lua_newuserdata(L, sizeof(class Feed *));
		assert(feed);

		*feed = f;
		luaL_getmetatable(L, "MajordomeFeed");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajFeedLib [] = {
	{"find", lfd_find},
	{NULL, NULL}
};

static int lfd_getContainer(lua_State *L){
	class Feed *feed= checkMajordomeFeed(L);
	lua_pushstring( L, feed->getWhereC() );
	return 1;
}

static int lfd_getName(lua_State *L){
	class Feed *feed= checkMajordomeFeed(L);
	lua_pushstring( L, feed->getName().c_str() );
	return 1;
}

static int lfd_isEnabled( lua_State *L ){
	class Feed *feed= checkMajordomeFeed(L);
	lua_pushboolean( L, feed->isEnabled() );
	return 1;
}

static int lfd_enabled( lua_State *L ){
	class Feed *feed= checkMajordomeFeed(L);
	feed->enable();
	return 0;
}

static int lfd_disable( lua_State *L ){
	class Feed *feed= checkMajordomeFeed(L);
	feed->disable();
	return 0;
}

static int lfd_getTable(lua_State *L){
	class Feed *feed= checkMajordomeFeed(L);
	lua_pushstring( L, feed->getTableName() );
	return 1;
}

static int lfd_getDatabase(lua_State *L){
	class Feed *feed= checkMajordomeFeed(L);
	class pgSQL **db = (class pgSQL **)lua_newuserdata(L, sizeof(class pgSQL *));
	assert(db);
	*db = feed->getDatabase();

	luaL_getmetatable(L, "MajordomepgSQL");
	lua_setmetatable(L, -2);

	return 1;
}

static const struct luaL_Reg MajFeedM [] = {
	{"getContainer", lfd_getContainer},
 	{"getName", lfd_getName},
	{"isEnabled", lfd_isEnabled},
	{"Enable", lfd_enabled},
	{"Disable", lfd_disable},
	{"getTable", lfd_getTable},
	{"getDatabase", lfd_getDatabase},
	{NULL, NULL}
};

void Feed::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeFeed", MajFeedM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeFeed", MajFeedLib );
}
