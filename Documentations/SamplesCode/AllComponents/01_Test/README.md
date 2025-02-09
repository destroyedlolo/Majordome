This is an *example* of a typical package. 
Have a look inside the provided files : they contain options applicable.
In a package's directory, you will find (in loading order) :

* **\*.topic** - MQTT topic definition
* **\*.lua** - Lua's task to associate with events (take a look at **DoNothing.lua** for the list of all parameters).
* **Init.lua** - a Lua script that is executed when the package is loaded. Notez-bien : It runs in Majordome's global context that is not shared with action scripts. Use **SelSharedVar** if something has to be shared.

---

- **MajordomeAbout.topic** defines `<ClientID>/About` topic
- **MajordomeAbout.lua** listen this topic and display Majordome's version and copyright when a message is received

---

- **Startup.lua** executed when the application is loaded

---

- **ClientTask.lua** is the main test task consuming **15s** timer and will be disabled/re-enable when a message arrives on **NoStations** topic, through **LuaTaskInterface.lua**

---

- **BipTask.lua** consumes **5s** and displays "*bip*" when launched.
- **TestTimerInterface.lua** will disable/re-enable **5s** timer when a message arrives on **NoStations** topic.
