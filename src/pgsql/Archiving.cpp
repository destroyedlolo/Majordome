#include "Archive.h"
#include "pgSQL.h"
#include "../Config.h"
#include "../Helpers.h"

#include <fstream>

#include <cstring>
#include <cassert>

Archive::Archive(const std::string &fch, std::string &where, std::string &name, lua_State *L): Object(fch, where, name), Handler(fch, where, name) {
}
