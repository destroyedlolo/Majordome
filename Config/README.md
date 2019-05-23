This directory contains some configuration examples.
Directories are loaded in order : 00_, 01_, ... and so on

* **Init.lua** (with capital 'I') is executed when the package is loaded, before any topic subscription or such.
* each scripts with **RunAtStartup** will be launched when everything is loaded so after topic subscription

CAUTION : Don't forget that each and every Lua scripts run in dedicated context so can only exchange data using **SelShared**, global variables are simply lost between scripts.
