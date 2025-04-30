#include "Archiving.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>
#include <sstream>

#include <cstring>
#include <cassert>

Archiving::Archiving(const std::string &fch, std::string &where, lua_State *L) : Object(fch, where), Handler(fch, where), Aggregation("Day"), kind(_kind::MINMAX), upto("1 day") {
	this->loadConfigurationFile(fch, where, L);
}

void Archiving::readConfigDirective( std::string &l ){
	std::string arg;

	if(!(arg = striKWcmp( l, "-->> when=" )).empty()){
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
	} else if(!(arg = striKWcmp( l, "-->> Database=" )).empty()){
		pgSQLCollection::iterator db;
		if( (db = config.pgSQLsList.find(arg)) != config.pgSQLsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tDatabase : %s", arg.c_str());
			this->db = db->second;
		} else {
			SelLog->Log('F', "\t\tDatabase '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> table=" )).empty()){
		this->TableName = arg;
		if(verbose)
			SelLog->Log('C', "\t\tTarget table : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> source=" )).empty()){
		this->SourceName = arg;
		if(verbose)
			SelLog->Log('C', "\t\tSource table : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "--> AggregateBy=" )).empty()){
		this->Aggregation = arg;
		if(verbose)
			SelLog->Log('C', "\t\tAggregation : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> Kind=" )).empty()){
		if(arg == "MinMax"){
			this->kind = _kind::MINMAX;
			if(verbose)
				SelLog->Log('C', "\t\tKind : MinMax");
		} else if(arg == "Sum"){
			this->kind = _kind::SUM;
			if(verbose)
				SelLog->Log('C', "\t\tKind : Sum");
		} else if(arg == "Delta"){
			this->kind = _kind::DELTA;
			if(verbose)
				SelLog->Log('C', "\t\tKind : Delta");
		} else {
			SelLog->Log('F', "\t\tUnknown archiving kind");
			exit(EXIT_FAILURE);
		}
	} else if(!(arg = striKWcmp( l, "-->> Keys=" )).empty()){
		std::istringstream iss(arg);
		std::string k;
		while(std::getline(iss, k, ','))
			this->keys.Add(k);

		if(verbose){
			k = "\t\tKeys : ";
			bool first = true;
			for(auto &i : this->keys){
				if(!first)
					k += ", ";
				k += i;
				first = false;
			}
			SelLog->Log('C', k.c_str());
		}
	} else if(!(arg = striKWcmp( l, "-->> UpTo=" )).empty()){
		this->upto= arg;
		if(verbose)
			SelLog->Log('C', "\t\tUp to : %s", arg.c_str());
	} else if(!(arg = striKWcmp( l, "-->> SuccessRDV=" )).empty()){
		EventCollection::iterator event;
		if( (event = config.EventsList.find(arg)) != config.EventsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tRendezvous '%s' add in successful list", arg.c_str());
			this->EventSuccessList.Add(event->second);
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
		} else {
			SelLog->Log('F', "\t\tRendezvous '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else
		this->Object::readConfigDirective(l);
}

const char *Archiving::getTableName(void){
	if(!this->TableName.empty())
		return(this->TableName.c_str());
	else
		return(this->getNameC());
}

bool Archiving::internalExec(void){
	if(!this->connect())
		return false;

	char *t;
	std::string cmd("INSERT INTO ");

	cmd += (t = PQescapeIdentifier(this->conn, this->getTableName(), strlen(this->getTableName())));
	PQfreemem(t);

	switch(this->kind){
	case _kind::MINMAX :
		cmd += " SELECT DATE_TRUNC(";
		cmd += (t = PQescapeLiteral(this->conn, this->Aggregation.c_str(), this->Aggregation.length()));
		PQfreemem(t);
		cmd += ", sample_time) AS frame";
		for(auto &i : this->keys){
			cmd += ", ";
			cmd += (t = PQescapeIdentifier(this->conn, i.c_str(), i.length()));
			PQfreemem(t);
		}

		cmd += ", MIN(value), MAX(value), AVG(value) FROM ";

		cmd += (t = PQescapeIdentifier(this->conn, this->SourceName.c_str(), this->SourceName.length()));
		PQfreemem(t);

		cmd += " WHERE sample_time::date < current_date - interval ";

		cmd += (t = PQescapeLiteral(this->conn, this->upto.c_str(), this->upto.length()));
		PQfreemem(t);

		cmd += " GROUP BY frame";
		for(auto &i : this->keys){
			cmd += ", ";
			cmd += (t = PQescapeIdentifier(this->conn, i.c_str(), i.length()));
			PQfreemem(t);
		}

		cmd += " ON CONFLICT DO NOTHING";
		break;
	case _kind::SUM:
		cmd += " SELECT DATE_TRUNC(";
		cmd += (t = PQescapeLiteral(this->conn, this->Aggregation.c_str(), this->Aggregation.length()));
		PQfreemem(t);
		cmd += ", sample_time) AS frame";
		for(auto &i : this->keys){
			cmd += ", ";
			cmd += (t = PQescapeIdentifier(this->conn, i.c_str(), i.length()));
			PQfreemem(t);
		}
		
		cmd += ", SUM(value) FROM ";

		cmd += (t = PQescapeIdentifier(this->conn, this->SourceName.c_str(), this->SourceName.length()));
		PQfreemem(t);

		cmd += " WHERE sample_time::date < current_date - interval ";

		cmd += (t = PQescapeLiteral(this->conn, this->upto.c_str(), this->upto.length()));
		PQfreemem(t);

		cmd += " GROUP BY frame";
		for(auto &i : this->keys){
			cmd += ", ";
			cmd += (t = PQescapeIdentifier(this->conn, i.c_str(), i.length()));
			PQfreemem(t);
		}

		cmd += " ON CONFLICT DO NOTHING";
		break;
	case _kind::DELTA :
		cmd += " SELECT DATE_TRUNC(";
		cmd += (t = PQescapeLiteral(this->conn, this->Aggregation.c_str(), this->Aggregation.length()));
		PQfreemem(t);
		cmd += ", sample_time) AS frame";
		for(auto &i : this->keys){
			cmd += ", ";
			cmd += (t = PQescapeIdentifier(this->conn, i.c_str(), i.length()));
			PQfreemem(t);
		}

		cmd += ", MAX(value)-MIN(value) FROM ";

		cmd += (t = PQescapeIdentifier(this->conn, this->SourceName.c_str(), this->SourceName.length()));
		PQfreemem(t);

		cmd += " WHERE sample_time::date < current_date - interval ";

		cmd += (t = PQescapeLiteral(this->conn, this->upto.c_str(), this->upto.length()));
		PQfreemem(t);

		cmd += " GROUP BY frame";
		for(auto &i : this->keys){
			cmd += ", ";
			cmd += (t = PQescapeIdentifier(this->conn, i.c_str(), i.length()));
			PQfreemem(t);
		}

		cmd += " ON CONFLICT DO NOTHING";
		break;
	}

	if(!this->doSQL(cmd.c_str())){
		SelLog->Log('E', "['%s'] %s", this->getNameC(), this->lastError());
		this->disconnect();
		return false;
	}

	this->disconnect();
	return true;
}

bool Archiving::execAsync(lua_State *){
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
