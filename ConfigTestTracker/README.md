This directory contains some configuration examples.
Directories are loaded in order : 00_, 01_, ... and so on

* Init.lua (with capital 'I') is executed at startup

CAUTION : Don't forget that each and every Lua scripts run in dedicated context so can only exchange data using **SelShared**, global variables are simply lost between scripts.
