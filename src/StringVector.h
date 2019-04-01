/* StringVector
 * As the name said
 */
#ifndef STRVECTOR_H
#define STRVECTOR_H

#include <vector>

class StringVector {
public:
	// Note : 
	// creating a vector of reference is not widely allowed
	// using pointers will crash as the task object used during configuration reading
	// will not survive to the said reading
	// Consequently, we are stored here only the name of the task.

	typedef std::vector<std::string> Entries;	// List of tasks'name to launch when a message arrives
protected :
	Entries list;

public :

	typedef Entries::iterator iterator;
	typedef Entries::const_iterator const_iterator;
	iterator begin() { return list.begin(); }
	iterator end() { return list.end(); }

	void Add( std::string t ){ this->list.push_back(t); }
	size_t size(){ return this->list.size(); }
};

#endif
