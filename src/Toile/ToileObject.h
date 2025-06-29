/* Generic class for Toile's objects
 *
 * - Handle visibility
 *
 * 23/06/2025 - LF - First version
 */

#ifndef TOILEOBJECT_H
#define TOILEOBJECT_H

#include "../Object.h"

#include <string>

class Renderer;
class Painting;

class ToileObject : virtual public Object {
	bool visible;

protected:
	Renderer *parentR;	// The parent is a renderer
	Painting *parentP;	// The parent is a painting

		/* Read configuration directive shared with all Toile's objects
		 * l -> string to read
		 *
		 * NOTEZ-BIEN : externals' like LuaExec or Object are not called in
		 *	case of unknown directive
		 */
	virtual bool readConfigDirective(std::string &l);

public:
	ToileObject();

		/* Object's own visibility */
	void Visibility(bool);
	bool getOwnVisibility(void) { return this->visible; }

		/* Cascaded visibility */
	bool isVisible(void);
	bool getDisplayed(void) { return this->isVisible(); }
};

#endif
