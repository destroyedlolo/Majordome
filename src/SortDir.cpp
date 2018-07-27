/* SortDir.cpp
 *
 * 26/07/2018 - LF - First version
 */

#include "SortDir.h"

#include <dirent.h>
#include <cerrno>

#include <system_error>
#include <algorithm>


void SortDir::readdircontent( const char *where ){
	DIR *dir;
	struct dirent *p;

	if( !(dir = opendir(where)) )
		throw std::system_error(errno, std::generic_category());

	while((p = readdir(dir))){
		string full( where );
		full += '/';
		full += p->d_name;

		if( this->accept( p->d_name, full ) )
			entries.push_back(  p->d_name );
	}

	closedir(dir);

	std::sort(entries.begin(), entries.end(), 
		[](string const &a, string const &b) -> bool {
		    return a < b;
		}
	);
}

