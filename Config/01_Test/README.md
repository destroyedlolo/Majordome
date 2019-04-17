This is an *example* of a typical package. 
Have a look inside provided files : they contain options applicable.
In a package's directory, you will find (in loading order) :

* **\*.topic** - MQTT topic definition
* **\*.lua** - Lua's task to associate with events (have a look on **DoNothing.lua** for the list of all parameters)
* **Init.lua** - a Lua script that is executed when the package is loaded. Notez-bien : it runs in Majordome's global context that is not shared with actions scripts. Use **SelShared** if something has to be shared.
