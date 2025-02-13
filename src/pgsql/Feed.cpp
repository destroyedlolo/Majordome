#include "Feed.h"
#include "pgSQL.h"

Feed::Feed(const std::string &fch, std::string &where, std::string &name): mpgConnection(db), Object(fch, where, name) {
}
