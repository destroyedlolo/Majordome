#include "Purge.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

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
				fd2 << this->getTri() << this->getName() << " -> " << event->second->getTri() << arg << ": SuccessRDV" << std::endl;
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
				fd2 << this->getTri() << this->getName() << " -> " << event->second->getTri() << arg << ": FailRDV" << std::endl;
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

bool Purge::internalExec(void){
	if(!this->connect())
		return false;

	return true;
}
