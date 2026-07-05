/* Generic class for Toile's objects
 * This kind of object is able to draw something.
 *
 * 23/06/2025 - LF - First version
 */

#ifndef TOILEOBJECT_H
#define TOILEOBJECT_H

#include "../Object.h"
#include "../LuaExec.h"

#include <Selene/SelGenericSurface.h>
#include <string>

class ToileObject : virtual public Object {
	ToileObject *parent;
protected:
	struct SelGenericSurface *surface;	// Selene's surface

		/* Read configuration directive shared with all Toile's objects
		 * l -> string to read
		 *
		 * NOTEZ-BIEN : externals' like LuaExec or Object are not called in
		 *	case of unknown directive
		 */
	virtual bool readConfigDirective(std::string &l);

		/* Check if the configuration is complete.
		 * Will crash if something is missing.
		 */
	virtual void assertSanity(void);

public:
	ToileObject();

		/* Accessors */
	struct SelGenericSurface *getSurface(){ return this->surface; }
	ToileObject *getParent(void){ return this->parent; };

		// Visibility is assumed for object without surfaces (like decoration)
	virtual bool isVisible(void){ return(!this->getSurface() || (this->getSurface() && this->getSurface()->cb->getVisibility(this->getSurface())) ); };

	virtual std::string getTri() = 0;

	virtual bool init(void) = 0;	// Initialise the object
	virtual void refreshAll(){};
};

#endif
