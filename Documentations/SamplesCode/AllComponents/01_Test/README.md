This is an *example* of a typical package. 
Have a look inside provided files : they contain options applicable.
In a package's directory, you will find (in loading order) :

* **\*.topic** - MQTT topic definition
* **\*.lua** - Lua's task to associate with events (have a look on **DoNothing.lua** for the list of all parameters)
* **Init.lua** - a Lua script that is executed when the package is loaded. Notez-bien : it runs in Majordome's global context that is not shared with actions scripts. Use **SelSharedVar** if something has to be shared.

---

- **MajordomeAbout.topic** define `<ClientID>/About` topic
- **MajordomeAbout.lua** listen this topic and display Majordome's version and copyright when a messgage is received

---

- **Startup.lua** executed when the application is loaded

---

- **ClientTask.lua** is the main test tasks consuming **15s** timer and **NoStations** topic.

