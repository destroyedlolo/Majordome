/* Selene.h
 *
 *	Interface with Selene
 *
 *	Copyright 2018-24 Laurent Faillie
 *
 */

#ifndef SELENE_H
#define SELENE_H

#include <Selene/libSelene.h>	/* Modules : the only part hardly linked */
#include <Selene/SeleneCore.h>	/* Selene's core functionalities */
#include <Selene/SelLog.h>		/* Logging : not really mandatory but very useful in most of the cases */
#include <Selene/SelLua.h>

extern struct SeleneCore *SeleneCore;
extern struct SelLog *SelLog;
extern struct SelLua *SelLua;

extern void initSelene(void);

#endif
