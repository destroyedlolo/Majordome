/* Class to abstract Toile's object containers.
 * Containers are objects that have child objects (like Renderer or Painting)
 * but are not drawing anything by their own.
 *
 * 29/06/2025 - LF - Creation
 */

#ifndef TOILECONTAINER_H
#define TOILECONTAINER_H

#include "ToileObject.h"

#include <vector>
#include <string>

class Decoration;

class ToileContainer {
protected:
	virtual bool readConfigDirective(std::string &l) = 0;

		/* Children
		 *
		 * Decorations are separated as must be render first, before
		 * other children.
		 *
		 * Notez-bien : children are not expected to overlap.
		 */
	std::vector<Decoration *> DecorationsList;		// List of decorations to apply
	std::vector<ToileObject *> ChildrenList;	// List of sub objects


public:

	/* ***
	 * Childs' management
	 * ***/
	void addDecoration( Decoration *t ){ this->DecorationsList.push_back(t); }
	void addChild( ToileObject *p ){ this->ChildrenList.push_back(p); }

};
#endif
