/* Abstract Action objects in Toile
 *
 * 17/01/2025 First version
 */
#ifndef TOILEACTION_H
#define TOILEACTION_H

class Config;

class ToileAction {
	virtual void MsgReceived(Config &, const char *name, const char *topic, const char *payload) = 0;
};

#endif
