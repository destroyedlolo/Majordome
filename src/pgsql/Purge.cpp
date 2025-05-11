#include "Purge.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <cstring>

Purge::Purge(const std::string &fch, std::string &where) : Object(fch, where), Handler(fch, where),upto("1 day"){
	this->loadConfigurationFile(fch, where);

	if(d2)
		fd2 << this->getFullId() << ".class: Purge" << std::endl;
}

void Purge::readConfigDirective( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> Database=" )).empty()){
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
	} else if(!(arg = striKWcmp( l, "-->> table=" )).empty()){
		this->TableName = arg;
		if(verbose)
			SelLog->Log('C', "\t\tTable : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> Keep=" )).empty()){
		this->upto= arg;
		if(verbose)
			SelLog->Log('C', "\t\tKeep : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> SuccessRDV=" )).empty()){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tRendezvous '%s' add in successful list", arg.c_str());
			this->EventSuccessList.Add(event->second);

			if(d2)
				fd2 << this->getFullId() << " -> " << event->second->getFullId() << ": SuccessRDV" << std::endl;
		} else {
			SelLog->Log('F', "\t\tRendezvous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> FailRDV=" )).empty()){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tRendezvous '%s' add in successful list", arg.c_str());
			this->EventFailList.Add(event->second);

			if(d2)
				fd2 << this->getFullId() << " -> " << event->second->getFullId() << ": FailRDV" << std::endl;
		} else {
			SelLog->Log('F', "\t\tRendezvous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(this->readConfigDirectiveNoData(l))
		;
	else
		this->Object::readConfigDirective(l);
}

const char *Purge::getTableName(void){
	if(!this->TableName.empty())
		return(this->TableName.c_str());
	else
		return(this->getNameC());
}

bool Purge::execAsync(lua_State *){
	bool ret = internalExec();

	if(!this->isQuiet())
		SelLog->Log('I', "['%s'] as %s run", this->getNameC(), ret ? "successfully" : "unsuccessfully");

	if(ret)
		for(auto &i : this->EventSuccessList)
			i->execHandlers();
	else
		for(auto &i : this->EventFailList)
			i->execHandlers();

	return ret;
}

/*
 * select * from test where sample_time::date < current_date - interval '3 months';
 *
 * 1517857
 *
 */

bool Purge::internalExec(void){
	if(!this->connect())
		return false;

	char *t;
	std::string cmd("DELETE FROM ");

	cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
	PQfreemem(t);

	cmd += " WHERE sample_time::date < current_date - interval ";

	cmd += (t = PQescapeLiteral(this->conn, this->upto.c_str(), this->upto.length()));
	PQfreemem(t);

	if(!this->doSQL(cmd.c_str())){
		SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
		this->disconnect();
		return false;
	}

	return true;
}
