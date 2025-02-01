/* Tracker object
 * 
 * 16/03/2019
 */

#ifndef TRACKER_H
#define TRACKER_H

#include "MayBeEmptyString.h"
#include "Handler.h"

class Tracker : public Handler {
	template <class T> class ObjCollection : public std::unordered_map<std::string, T> {};

		/* notifications */
};
