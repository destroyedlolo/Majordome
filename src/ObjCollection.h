/* Collection of objects
 *
 * 01/02/2025 Emancipate from Config.h
 */

#ifndef OBJCOLLECTION_H
#define OBJCOLLECTION_H

#include <unordered_map>

template <class T> class ObjCollection : public std::unordered_map<std::string, T> {};

#endif
