/* SortDir.h
 *
 * 	Read directory content
 * 	(the old way as <filesystem> is not yet supported)
 *
 * 	26/07/2018 - LF - First (experimental) version
 */
#ifndef SORTDIR_H
#define SORTDIR_H

#include "StringVector.h"

class SortDir : public StringVector {
protected:
		/* File acceptance :
		 * -> fch : filename
		 * -> full : full path of this file
		 * <- boolean : accepted (true) or rejected (false)
		 *
		 * Here, ignore everything starting by
		 * a dot '.' and may be overload by derived classes
		 */
	virtual bool accept( const char *fch, std::string &full ){
		return(*fch != '.');
	}

		/* Can't be made directly in the constructor otherwise 
		 * virtual are not assumed ... GGGGrrrrrrr !
		 */
	void readdircontent( const char *where );
	void readdircontent( std::string &where){ this->readdircontent(where.c_str()); }

public:
	SortDir(const char *where){
		this->readdircontent( where );
	}
	SortDir() {};

	virtual void sort( void );
};

#endif
