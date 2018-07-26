/* SortDir.h
 *
 * 	Read directory content
 * 	(the old way as <filesystem> is not yet supported)
 *
 * 	26/07/2018 - LF - First (experimental) version
 */
#ifndef SORTDIR_H
#define SORTDIR_H

#include <vector>
#include <string>

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
	void readdircontent( const char *where );

public:
	SortDir(const char *where){
		this->readdircontent( where );
	}
	SortDir() {};
};

#endif
