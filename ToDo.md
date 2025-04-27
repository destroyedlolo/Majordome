# Milestones

## General
- [X] Ensure inherance
- [ ] D2 generation

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

--- Bugs

- Pour les objets qui ont des scripts de validation ("*MinMax", "*feed"), la donnée doit être rejetée si le script échoue

--- Migration to v6

MAJORDOME_TRIGGER replaced by MAJORDOME_TOPIC_NAME

--- Improvement

needs : checking will not work if at same level (or lower).
	- remove existance checking in LuaExec
	- add checking in Config::SanityChecks

--- Futures

(Named)MinMax, [Named]Feed handlers to run asynchronously.

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


