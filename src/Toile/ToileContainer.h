/* Class to abstract Toile's object containers.
 * Containers are objects that have child objects (like Renderer or Painting)
 *
 * 29/06/2025 - LF - Creation
 */

#ifndef TOILECONTAINER_H
#define TOILECONTAINER_H

#include "ToileObject.h"
#include "../LuaExec.h"

#include <Selene/SelGenericSurface.h>
#include <vector>

class Decoration;

class ToileContainer : virtual public ToileObject, virtual public LuaExec {
protected:
	struct SelGenericSurface *surface;	// Selene's surface
	virtual bool readConfigDirective(std::string &l) = 0;

		/* Children
		 *
		 * Decorations are separated as must be render first, before
		 * other children.
		 *
		 * Notez-bien : children are not expected to overlap.
		 */
	std::vector<Decoration *> DecorationsList;		// List of decorations to apply
	std::vector<ToileContainer *> ContainersList;	// List of sub containers


public:
	ToileContainer(): surface(NULL){}

	struct SelGenericSurface *getSurface(){ return this->surface; }

	/* ***
	 * Childs' management
	 * ***/
	void addDecoration( Decoration *t ){ this->DecorationsList.push_back(t); }
	void addChild( ToileContainer *p ){ this->ContainersList.push_back(p); }

};
#endif
