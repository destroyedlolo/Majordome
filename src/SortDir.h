/* SortDir.h
 *
 * 	Read directory content
 * 	(the old way as <filesystem> is not yet supported)
 *
 * 	26/07/2018 - LF - First (experimental) version
 */
#ifndef SORTDIR_H
#define SORTDIR_H

#include <dirent.h>
#include <cerrno>

#include <vector>
#include <string>
#include <system_error>
#include <algorithm>

using namespace std;

class SortDir {

	typedef vector<string> DirEntries;
	DirEntries entries;

public :
	typedef DirEntries::iterator iterator;
	typedef DirEntries::const_iterator const_iterator;
	iterator begin() { return entries.begin(); }
	iterator end() { return entries.end(); }
	
protected:
		/* File acceptance : in this version, ignore everything starting by
		 * a dot '.'
		 */
	virtual bool accept( const char *fch, const char *dir ){
		return(*fch != '.');
	}

		/* Can't be made directly in the constructor otherwise 
		 * virtual are not assumed ... GGGGrrrrrrr !
		 */
	void readdircontent( const char *where ){
		DIR *dir;
		struct dirent *p;

		if( !(dir = opendir(where)) )
			throw std::system_error(errno, std::generic_category());

		while((p = readdir(dir))){
			if( this->accept( p->d_name, where ) )
				entries.push_back(  p->d_name );
		}

		closedir(dir);

		std::sort(entries.begin(), entries.end(), 
			[](string const &a, string const &b) -> bool {
			    return a < b;
			}
		);
	}

public:
	SortDir(const char *where){
		this->readdircontent( where );
	}
	SortDir() {};
};

#endif
