/* Generic class for Toile's objects
 * This kind of object is able to draw something.
 *
 * 23/06/2025 - LF - First version
 */

#ifndef TOILEOBJECT_H
#define TOILEOBJECT_H

#include "../Object.h"
#include "../LuaExec.h"

#include <string>

class ToileContainer;

class ToileObject : virtual public Object {
	bool visible;

	ToileContainer *parent;
protected:

		/* Read configuration directive shared with all Toile's objects
		 * l -> string to read
		 *
		 * NOTEZ-BIEN : externals' like LuaExec or Object are not called in
		 *	case of unknown directive
		 */
	virtual bool readConfigDirective(std::string &l);

public:
	ToileObject();

		/* Accessors */
	ToileContainer *getParent(void){ return this->parent; };

	virtual std::string getTri() = 0;

		/* Object's own visibility */
	void Visibility(bool);
	bool getOwnVisibility(void) { return this->visible; }

		/* Cascaded visibility */
	bool isVisible(void);
	bool getDisplayed(void) { return this->isVisible(); }

};

#endif
