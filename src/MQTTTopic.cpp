#include <iostream>
#include <fstream>
#include <cstring>

#include "Components.h"
#include "MQTTTopic.h"

MQTTTopic::MQTTTopic( std::string &fch ){
	/* determine the name from the filename */
	std::string name = fch;
	const size_t last_slash_idx = name.find_last_of("/");	// Filename only
	if(std::string::npos != last_slash_idx)
		name.erase(0, last_slash_idx + 1);

	const size_t period_idx = name.rfind('.');	// Remove extension if present.
	if (std::string::npos != period_idx)
		name.erase(period_idx);

	/* Reading file's content */
printf("*L* '%s'\n", fch.c_str());

	std::ifstream file;
	file.exceptions ( std::ios::eofbit | std::ios::failbit ); // No need to check failbit
	try {
		std::string l;

		file.open(fch);
		while( std::getline( file, l) ){
			MayBeEmptyString arg;

			if( !!(arg = striKWcmp( l, "name=" ))){
				printf(">> '%s'\n", arg.c_str());
			} else
				puts(l.c_str());
		}
	} catch(const std::ifstream::failure &e){
		if(!file.eof()){
			publishLog('F', "%s : %s", fch.c_str(), strerror(errno) );
			exit(EXIT_FAILURE);
		}
	}

	file.close();
}
