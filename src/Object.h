/* Generic class for objects
 *
 * 27/07/2018 - LF - First version
 */

#ifndef OBJECT_H
#define OBJECT_H

#include <iostream>
#include <string>

class Object {
	bool disabled;
	bool quiet;

protected:
	std::string name;
	std::string where;
	std::string description;	// Description to be presented as tooltips
	std::string embeddedCom;	// Small embedded comment (like units)
	std::string group;			// to group object during D2 generation

		/* Load the configuration file.
		 * Should have been done within the constructor, but in this case,
		 * derived methods are not called.
		 *
		 * Need to be explicitly called from derived's constructor.
		 */
	void loadConfigurationFile(const std::string &fch, std::string &where, std::stringstream *buffer=NULL);

		/* Read configuration directive
		 * l -> string to read
		 *
		 * NOTEZ-BIEN : Object's one will fail if the option is not recognized.
		 * Consequently, it has to be called lastly.
		 */
	virtual void readConfigDirective(std::string &l);

	virtual std::string getTri() = 0;	// Get type's trigram (needed for d2)
public:
	Object() : disabled(false), quiet(false){};
	Object(const std::string &fch, std::string &where);

//	Object& operator=(const Object&) = default;	/* avoid "defaulted move" warning */

	void enable( void ){ this->disabled = false; };
	void disable( void ){ this->disabled = true; };
	bool isEnabled( void ){ return !this->disabled; };

		// Remove some messages
	void beQuiet( void ){ this->quiet = true; };
	void beNoisy( void ){ this->quiet = false; };
	bool isQuiet( void ){ return this->quiet; };

	std::string &getName( void ){ return this->name; };	// Object's name
	const char *getNameC( void ){ return this->name.c_str(); };
	std::string &getWhere( void ){ return this->where; }; // Object's container (it's father directory)
	const char *getWhereC( void ){ return this->where.c_str(); };
	std::string getContainer( void );
	std::string getFullId( void );

	/*
	 * determine the name from the filename
	 * -> fch : filename (optionally including its path)
	 * <- name : extracted name
	 */
	void extrName( const std::string &fch, std::string &name );

};

#endif
