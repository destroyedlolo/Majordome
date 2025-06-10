#include "AggregatedFeed.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

AggregatedFeed::AggregatedFeed(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where), minmax(NULL), nminmax(NULL), mkminmax(NULL), figure(_which::AVG), noEmpty(false){
	this->loadConfigurationFile(fch, where,L);
	if(d2)
		fd2 << this->getFullId() << ".class: AggregatedFeed" << std::endl;

		/* Sanity checks */
	if(!this->db){
		SelLog->Log('F', "[%s] No database defined", this->getNameC());
		exit(EXIT_FAILURE);
	}

	if(!this->minmax && !this->nminmax && !this->mkminmax){
		SelLog->Log('F', "[%s] No source defined", this->getNameC());
		exit(EXIT_FAILURE);
	}
}

	/* We can't reuse Feed's as we are not waiting for topics,
	 * only events.
	 */
void AggregatedFeed::readConfigDirective( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> table=" )).empty()){
		this->TableName = arg;
		if(::verbose)
			SelLog->Log('C', "\t\tTable : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> Database=" )).empty()){
		pgSQLCollection::iterator db;
		if( (db = config.pgSQLsList.find(arg)) != config.pgSQLsList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tDatabase : %s", arg.c_str());
			this->db = db->second;
			if(d2)
				fd2 << this->getFullId() << " <- " << db->second->getFullId() << ": { class: ldatabase }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tDatabase '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> from MinMax=" )).empty()){
		if(this->minmax || this->nminmax || this->mkminmax){
			SelLog->Log('F', "[%s] a source has been already defined", this->getNameC());
			exit(EXIT_FAILURE);
		}

		MinMaxCollection::iterator mm;
		if( (mm = config.MinMaxList.find(arg)) != config.MinMaxList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tMinMax : %s", arg.c_str());
			this->minmax = mm->second;
			if(d2)
				fd2 << this->getFullId() << " <- " << mm->second->getFullId() << ": from { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tMinMax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> from NamedMinMax=" )).empty()){
		if(this->minmax || this->nminmax || this->mkminmax){
			SelLog->Log('F', "[%s] a source has been already defined", this->getNameC());
			exit(EXIT_FAILURE);
		}

		NamedMinMaxCollection::iterator mm;
		if( (mm = config.NamedMinMaxList.find(arg)) != config.NamedMinMaxList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tNamedMinMax : %s", arg.c_str());
			this->nminmax = mm->second;
			if(d2)
				fd2 << this->getFullId() << " <- " << mm->second->getFullId() << ": from { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tNamedMinMax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> from MultiKeysMinMax=" )).empty()){
		if(this->minmax || this->nminmax || this->mkminmax){
			SelLog->Log('F', "[%s] a source has been already defined", this->getNameC());
			exit(EXIT_FAILURE);
		}

		MultiKeysMinMaxCollection::iterator mm;
		if( (mm = config.MultiKeysMinMaxList.find(arg)) != config.MultiKeysMinMaxList.end()){
			if(::verbose)
				SelLog->Log('C', "\t\tMultiKeysMinMax : %s", arg.c_str());
			this->mkminmax = mm->second;
			if(d2)
				fd2 << this->getFullId() << " <- " << mm->second->getFullId() << ": from { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tMultiKeysMinMax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> figure=" )).empty()){
		if(arg == "Average" || arg == "AVG")
			this->figure = _which::AVG;
		else if(arg == "Min")
			this->figure = _which::MIN;
		else if(arg == "Max")
			this->figure = _which::MAX;
		else if(arg == "Sum")
			this->figure = _which::SUM;
		else if(arg == "MMA")
			this->figure = _which::MMA;
		else {
			SelLog->Log('F', "\t\tFigure '%s' is not known", arg.c_str());
			exit(EXIT_FAILURE);
		}

		if(::verbose)
			SelLog->Log('C', "\t\tFigure : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> preprocess=" )).empty()){
		this->func = arg;
		if(::verbose)
			SelLog->Log('C', "\t\tPreprocessing function : %s", arg.c_str());
	} else if( l == "-->> ignore empty" ){
		this->noEmpty = true;
		if(::verbose)
			SelLog->Log('C', "\t\tIgnore empty records");

	} else if(this->readConfigDirectiveData(l))
		;
	else if(this->readConfigDirectiveNoData(l))
		;
	else 
		this->LuaExec::readConfigDirective(l);

}

void AggregatedFeed::feedState( lua_State *L ){
	class AggregatedFeed **feed= (class AggregatedFeed **)lua_newuserdata(L, sizeof(class AggregatedFeed *));
	assert(feed);

	lua_pushstring( L, this->getNameC() );	// Push the name of the tracker
	lua_setglobal( L, "MAJORDOME_AGGREGATEDFEED" );

	*feed = this;
	luaL_getmetatable(L, "MajordomeAggregatedFeed");
	lua_setmetatable(L, -2);
	lua_setglobal( L, "MAJORDOME_Myself" );
}

bool AggregatedFeed::execAsync(lua_State *L){
	LuaExec::boolRetCode rc;
	lua_Number val, max = 0, avg = 0;

printf("**** 00000 : %d\n", lua_gettop(L));
	bool r = this->LuaExec::execSync(L, &rc, &val);
printf("**** 00001 : %d\n", lua_gettop(L));
SelLua->dumpstack(L);

	if( rc != LuaExec::boolRetCode::RCfalse ){	// data not rejected
		if(this->isVerbose())
			SelLog->Log('T', "['%s'] accepting", this->getNameC());

#if DEBUG
		if(this->isVerbose()){
			if(this->minmax)
				this->minmax->dump();
			else if(this->nminmax)
				this->nminmax->dump();
			else
				this->mkminmax->dump();
		}
#endif

		char *t;
		if(this->minmax){
			if(isnan(val) || this->figure == _which::MMA){	// data not forced
				if(this->noEmpty && this->minmax->isEmpty()){	// Let it work if the data is forced
					lua_close(L);
					return false;
				}

				switch(this->figure){
				case _which::AVG:
					val = this->minmax->getAverage();
					break;
				case _which::MIN:
					val = this->minmax->getMin();
					break;
				case _which::MAX:
					val = this->minmax->getMax();
					break;
				case _which::SUM:
					val = this->minmax->getSum();
					break;
				default: /* _which::MMA */
					val = this->minmax->getMin();
					max = this->minmax->getMax();
					avg = this->minmax->getAverage();
					break;
				}
				this->minmax->Clear();
			}

			if(!this->func.empty()){	// Need to preprocess the data
				lua_getglobal(L, this->func.c_str() );
				lua_pushnumber(L, val);
				if(this->figure == _which::MMA){
					lua_pushnumber(L, max);
					lua_pushnumber(L, avg);
					if(lua_pcall(L, 3, 3, 0)){
						SelLog->Log('E', "['%s'/'%s'] %s", this->getNameC(), this->func.c_str(), lua_tostring(L, -1));
						lua_pop(L, 1);
						lua_close(L);
						return false;
					}
					val = lua_tonumber(L, -3);
					max = lua_tonumber(L, -2);
					avg = lua_tonumber(L, -1);
					lua_pop(L, 3);
				} else {
					if(lua_pcall(L, 1, 1, 0)){
						SelLog->Log('E', "['%s'/'%s'] %s", this->getNameC(), this->func.c_str(), lua_tostring(L, -1));
						lua_pop(L, 1);
						lua_close(L);
						return false;
					}
					val = lua_tonumber(L, -1);
					lua_pop(L, 1);
				}
			}

			/* Build SQL request */
			if(!this->connect()){
				lua_close(L);
				return false;
			}

			std::string cmd("INSERT INTO ");
			cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
			PQfreemem(t);

			cmd += " VALUES ( now(), ",
			cmd += std::to_string(val);
			if(this->figure == _which::MMA){
				cmd += "," + std::to_string(max);
				cmd += "," + std::to_string(avg);
			}
			cmd += " )";

			if(!this->doSQL(cmd.c_str()))
				SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
		} else if(this->nminmax){
			/* Build SQL request */
			if(!this->connect()){
				lua_close(L);
				return false;
			}

			for(auto & it: this->nminmax->getEmptyList()){	// Iterating against keys
				if(this->noEmpty && this->nminmax->isEmpty(it.first))
					continue;

				switch(this->figure){
				case _which::AVG:
					val = this->nminmax->getAverage(it.first);
					break;
				case _which::MIN:
					val = this->nminmax->getMin(it.first);
					break;
				case _which::MAX:
					val = this->nminmax->getMax(it.first);
					break;
				case _which::SUM:
					val = this->nminmax->getSum(it.first);
					break;
				default: /* _which::MMA */
					val = this->nminmax->getMin(it.first);
					max = this->nminmax->getMax(it.first);
					avg = this->nminmax->getAverage(it.first);
					break;
				}
				this->nminmax->Clear(it.first);

				if(!this->func.empty()){	// Need to preprocess the data
					lua_getglobal(L, this->func.c_str() );
					lua_pushnumber(L, val);
					if(this->figure == _which::MMA){
						lua_pushnumber(L, max);
						lua_pushnumber(L, avg);
						lua_pushstring(L, it.first.c_str());
						if(lua_pcall(L, 4, 3, 0)){
							SelLog->Log('E', "['%s'/'%s'] %s", this->getNameC(), this->func.c_str(), lua_tostring(L, -1));
							lua_pop(L, 1);
							continue;
						}
						val = lua_tonumber(L, -3);
						max = lua_tonumber(L, -2);
						avg = lua_tonumber(L, -1);
						lua_pop(L, 3);
					} else {
						lua_pushstring(L, it.first.c_str());
						if(lua_pcall(L, 2, 1, 0)){
							SelLog->Log('E', "['%s'/'%s'] %s", this->getNameC(), this->func.c_str(), lua_tostring(L, -1));
							lua_pop(L, 1);
							continue;	// data not processed
						}
						val = lua_tonumber(L, -1);
						lua_pop(L, 1);
					}
				}

				std::string cmd("INSERT INTO ");
				cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
				PQfreemem(t);

				cmd += " VALUES ( now(), ",
				cmd += (t = PQescapeLiteral(this->conn, it.first.c_str(), it.first.length()));
				PQfreemem(t);
	
				cmd += ", ";
				cmd += std::to_string(val);
				if(this->figure == _which::MMA){
					cmd += "," + std::to_string(max);
					cmd += "," + std::to_string(avg);
				}
				cmd += " )";

				if(!this->doSQL(cmd.c_str()))
					SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
			}
		} else {
			/* Build SQL request */
			if(!this->connect()){
				lua_close(L);
				return false;
			}

			for(auto & it: this->mkminmax->getEmptyList()){	// Iterating against keys
				if(this->noEmpty && this->mkminmax->isEmpty(it.first))
					continue;

				switch(this->figure){
				case _which::AVG:
					val = this->mkminmax->getAverage(it.first);
					break;
				case _which::MIN:
					val = this->mkminmax->getMin(it.first);
					break;
				case _which::MAX:
					val = this->mkminmax->getMax(it.first);
					break;
				case _which::SUM:
					val = this->mkminmax->getSum(it.first);
					break;
				default: /* _which::MMA */
					val = this->mkminmax->getMin(it.first);
					max = this->mkminmax->getMax(it.first);
					avg = this->mkminmax->getAverage(it.first);
					break;
				}
				this->mkminmax->Clear(it.first);

				if(!this->func.empty()){	// Need to preprocess the data
					lua_getglobal(L, this->func.c_str() );
					lua_pushnumber(L, val);
					if(this->figure == _which::MMA){
						lua_pushnumber(L, max);
						lua_pushnumber(L, avg);

						lua_newtable(L);
						size_t i=0;
						for(auto &k: it.first){
							lua_pushstring( L, k.c_str() );
							lua_seti(L, -2, static_cast<lua_Integer>(++i));
						}

puts("**** Avant");
SelLua->dumpstack(L);
						if(lua_pcall(L, 4, 3, 0)){
							SelLog->Log('E', "['%s'/'%s'] %s", this->getNameC(), this->func.c_str(), lua_tostring(L, -1));
							lua_pop(L, 1);
							continue;
						}
						val = lua_tonumber(L, -3);
						max = lua_tonumber(L, -2);
						avg = lua_tonumber(L, -1);
						lua_pop(L, 3);
					} else {
						lua_newtable(L);
						size_t i=0;
						for(auto &k: it.first){
							lua_pushstring( L, k.c_str() );
							lua_seti(L, -2, static_cast<lua_Integer>(++i));
						}

						if(lua_pcall(L, 2, 1, 0)){
							SelLog->Log('E', "['%s'/'%s'] %s", this->getNameC(), this->func.c_str(), lua_tostring(L, -1));
							lua_pop(L, 1);
							continue;	// data not processed
						}
						val = lua_tonumber(L, -1);
						lua_pop(L, 1);
					}
				}

#if 0
				std::string cmd("INSERT INTO ");
				cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
				PQfreemem(t);

				cmd += " VALUES ( now(), ",
				cmd += (t = PQescapeLiteral(this->conn, it.first.c_str(), it.first.length()));
				PQfreemem(t);
	
				cmd += ", ";
				cmd += std::to_string(val);
				if(this->figure == _which::MMA){
					cmd += "," + std::to_string(max);
					cmd += "," + std::to_string(avg);
				}
				cmd += " )";

				if(!this->doSQL(cmd.c_str()))
					SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
#endif
			}
		}
	
		this->disconnect();		
	} else
		SelLog->Log('D', "['%s'] Data rejected", this->getNameC());

	lua_close(L);
	return r;
}

	/*****
	 * Lua exposed functions
	 *****/

static class AggregatedFeed *checkMajordomeAggregatedFeed(lua_State *L){
	class AggregatedFeed **r = (class AggregatedFeed **)SelLua->testudata(L, 1, "MajordomeAggregatedFeed");
	luaL_argcheck(L, r != NULL, 1, "'MajordomeAggregatedFeed' expected");
	return *r;
}

static int lfd_find(lua_State *L){
	const char *name = luaL_checkstring(L, 1);

	try {
		class AggregatedFeed *f = config.AggregatedFeedsList.at( name );
		class AggregatedFeed **feed = (class AggregatedFeed **)lua_newuserdata(L, sizeof(class AggregatedFeed *));
		assert(feed);

		*feed = f;
		luaL_getmetatable(L, "MajordomeAggregatedFeed");
		lua_setmetatable(L, -2);

		return 1;
	} catch( std::out_of_range &e ){	// Not found 
		return 0;
	}
}

static const struct luaL_Reg MajAggregatedFeedLib [] = {
	{"find", lfd_find},
	{NULL, NULL}
};

static int lfd_getContainer(lua_State *L){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	lua_pushstring( L, feed->getWhereC() );
	return 1;
}

static int lfd_getName(lua_State *L){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	lua_pushstring( L, feed->getName().c_str() );
	return 1;
}

static int lfd_isEnabled( lua_State *L ){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	lua_pushboolean( L, feed->isEnabled() );
	return 1;
}

static int lfd_enabled( lua_State *L ){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	feed->enable();
	return 0;
}

static int lfd_disable( lua_State *L ){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	feed->disable();
	return 0;
}

static int lfd_getTable(lua_State *L){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	lua_pushstring( L, feed->getTableName() );
	return 1;
}

static int lfd_getDatabase(lua_State *L){
	class AggregatedFeed *feed= checkMajordomeAggregatedFeed(L);
	class pgSQL **db = (class pgSQL **)lua_newuserdata(L, sizeof(class pgSQL *));
	assert(db);
	*db = feed->getDatabase();

	luaL_getmetatable(L, "MajordomepgSQL");
	lua_setmetatable(L, -2);

	return 1;
}

static const struct luaL_Reg MajAggregatedFeedM [] = {
	{"getContainer", lfd_getContainer},
 	{"getName", lfd_getName},
	{"isEnabled", lfd_isEnabled},
	{"Enable", lfd_enabled},
	{"Disable", lfd_disable},
	{"getTable", lfd_getTable},
	{"getDatabase", lfd_getDatabase},
	{NULL, NULL}
};

void AggregatedFeed::initLuaInterface( lua_State *L ){
	SelLua->objFuncs( L, "MajordomeAggregatedFeed", MajAggregatedFeedM );
	SelLua->libCreateOrAddFuncs( L, "MajordomeAggregatedFeed", MajAggregatedFeedLib );
}
