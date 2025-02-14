# Milestones

## General
- [X] Ensure inherance

## Devices
### LCD

- [x] Subsurface
- [ ] Sub subsurface
- [ ] Field
- [ ] Series
- [ ] Layer / Mask

### OLed

### Curses

### DRM

---

Dans tous les \*::feedState(), supprimer les find().
Voir Feed.

--- Migration to v6

MAJORDOME_TRIGGER replaced by MAJORDOME_TOPIC_NAME

--- Doc

Postgresql :
    Arch : 
    Gentoo : 
        package.use : dev-db/postgresql -server
        emerge -va postgresql
    Arch :
        postgresql-libs
--- Improvement

needs : checking will not work if at same level (or lower).
	- remove existance checking in LuaExec
	- add checking in Config::SanityChecks

--- Futures

(Named)MinMax handler to run asynchronously.

---
Add a new Object :
- Config.h
- SubConfigDir : extension
- LuaExec.h : needed / required / feed
- LuaExec.cpp : feedbyNeeded()
- Majordome.cpp : SelLua->AddStartupFunc()

---

static_cast - Base -> Derived
dynamic_cast - Derived -> Base


