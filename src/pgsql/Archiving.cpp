#include "Archiving.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>
#include <sstream>

#include <cstring>
#include <cassert>

Archiving::Archiving(const std::string &fch, std::string &where, std::string &name, lua_State *L) : Object(fch, where, name), Handler(fch, where, name), Aggregation("Day"), kind(_kind::MINMAX) {
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

		file.close();
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			SelLog->Log('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}
}

void Archiving::readConfigDirective( std::string &l, std::string &name, bool &nameused ){
	MayBeEmptyString arg;

	if( !!(arg = striKWcmp( l, "-->> when=" ))){
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
	} else if(!!(arg = striKWcmp( l, "-->> Database=" ))){
		pgSQLCollection::iterator db;
		if( (db = config.pgSQLsList.find(arg)) != config.pgSQLsList.end()){
			if(verbose)
				SelLog->Log('C', "\t\tDatabase : %s", arg.c_str());
			this->db = db->second;
		} else {
			SelLog->Log('F', "\t\tDatabase '%s' is not (yet ?) defined", arg.c_str());
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> table=" ))){
		this->TableName = arg;
		if(verbose)
			SelLog->Log('C', "\t\tTarget table : %s", arg.c_str());
	} else if(!!(arg = striKWcmp( l, "-->> source=" ))){
		this->SourceName = arg;
		if(verbose)
			SelLog->Log('C', "\t\tSource table : %s", arg.c_str());
	} else if(!!(arg = striKWcmp( l, "-->> AggregateBy=" ))){
		this->Aggregation = arg;
		if(verbose)
			SelLog->Log('C', "\t\tAggregation : %s", arg.c_str());
	} else if(!!(arg = striKWcmp( l, "-->> Kind=" ))){
		if(arg == "MinMax"){
			this->kind = _kind::MINMAX;
			if(verbose)
				SelLog->Log('C', "\t\tKind : MinMax");
		} else if(arg == "Delta"){
			this->kind = _kind::DELTA;
			if(verbose)
				SelLog->Log('C', "\t\tKind : Delta");
		} else {
			SelLog->Log('F', "\t\tUnknown archiving kind");
			exit(EXIT_FAILURE);
		}
	} else if(!!(arg = striKWcmp( l, "-->> Keys=" ))){
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
				k+=i;
				first = false;
			}
			SelLog->Log('C', k.c_str());
		}
	} else if(!!(arg = striKWcmp( l, "-->> UpTo=" ))){
		this->Aggregation = arg;
		if(verbose)
			SelLog->Log('C', "\t\tUp to : %s", arg.c_str());
	} else
		this->Object::readConfigDirective(l, name, nameused);
}
