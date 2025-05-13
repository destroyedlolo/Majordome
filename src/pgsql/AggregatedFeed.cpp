#include "AggregatedFeed.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

AggregatedFeed::AggregatedFeed(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where), minmax(NULL), figure(_which::AVG){
	this->loadConfigurationFile(fch, where,L);
	if(d2)
		fd2 << this->getFullId() << ".class: AggregatedFeed" << std::endl;

		/* Sanity checks */
	if(!this->db){
		SelLog->Log('F', "[%s] No database defined", this->getNameC());
		exit(EXIT_FAILURE);
	}

	if(!this->minmax){
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
		if(verbose)
			SelLog->Log('C', "\t\tTable : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> Database=" )).empty()){
		pgSQLCollection::iterator db;
		if( (db = config.pgSQLsList.find(arg)) != config.pgSQLsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tDatabase : %s", arg.c_str());
			this->db = db->second;
			if(d2)
				fd2 << this->getFullId() << " <- " << db->second->getFullId() << ": { class: ldatabase }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tDatabase '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> from MinMax=" )).empty()){
		MinMaxCollection::iterator mm;
		if( (mm = config.MinMaxList.find(arg)) != config.MinMaxList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tMinMax : %s", arg.c_str());
			this->minmax = mm->second;
			if(d2)
				fd2 << this->getFullId() << " <- " << mm->second->getFullId() << ": from { class: llink }" << std::endl;
		} else {
			SelLog->Log('F', "\t\tMinMax '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> figure=" )).empty()){
		if(arg == "Average" || arg == "AVG")
			this->figure = _which::AVG;
		else if(arg == "Min")
			this->figure = _which::MIN;
		else if(arg == "Max")
			this->figure = _which::MAX;
		else {
			SelLog->Log('F', "\t\tFigure '%s' is not known", arg.c_str());
			exit(EXIT_FAILURE);
		}

		if(verbose)
			SelLog->Log('C', "\t\tFigure : %s", arg.c_str());
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
	lua_Number val;

	bool r = this->LuaExec::execSync(L, &rc, &val);
	if( rc != LuaExec::boolRetCode::RCfalse ){	// data not rejected
		if(debug && !this->isQuiet())
			SelLog->Log('T', "['%s'] accepting", this->getNameC());

		if(isnan(val)){	// data not forced
#if DEBUG
			if(debug && !this->isQuiet())
				this->minmax->dump();
#endif
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
			}
			this->minmax->Clear();
		}

		/* Build SQL request */
		if(!this->connect()){
			lua_close(L);
			return false;
		}

		char *t;
		if(this->minmax){
			std::string cmd("INSERT INTO ");
			cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
			PQfreemem(t);

			cmd += " VALUES ( now(), ",
			cmd += std::to_string(val);
			cmd += " )";

			if(!this->doSQL(cmd.c_str()))
				SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
		}
	
		this->disconnect();		
	} else
		SelLog->Log('D', "['%s'] Data rejected", this->getNameC());

	lua_close(L);
	return r;
}
