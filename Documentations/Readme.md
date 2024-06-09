Majordome's configuration
=========================

This little page explaines how to configure **Majordome**.<br>
Some examples are provided also in `Config` directory at the root of this project.

# Majordome command line options

## General options
- `-h` : onling helps about known options (mostly what is bellow)
- `-f` : specify Majordome's configuration file. If not provided, the default it `/usr/local/etc/Majordome.conf`
- `-t` : tests configuration file and exits. Ensures your configuration is syntax correct and understood as you're expecting by Majordome.

## Verbosity
- `-q` : be quiet, remove all messages but script generated ones
- `-v` : enable verbose messages
- `-r` : enable trace messages, even more verbose, noisy
- `-V` : silence topic arrival messages. Avoid verbosity to be polluted but incoming messages tracking which is very noisy.

# Majordome own configuration file

A configuration file instructs Majordome about its basic configuration and where are located automation objects.<br>
Lines starting by a hash sign `#` are considered as comment and, as such, ignored.

> [!TIP]
> Thanks to `-f`, it's possible to specify the configuration file to use, consequently, it's possible to have several instances running with totally separate configuration.

## Known configuration directives
### Broker_URL
**MQTT Broker** to connect to. With *Paho* syntax that is : `tcp://*fqdn_hostname*:*port*`.<br>
Example (which is the default value) :<br>
`Broker_URL=tcp://localhost:1883`

### ClientID
**Client ID** to uniquely identify to the broker.
> [!CAUTION]
> In case you have more than one Majordome instance connected to a single broker, you **MUST** set a unique ID per instance.

If not set, the Client ID will be `Majordome-*Hostname*-*pid*`.

Example :<br>
`ClientID=Majordome`

### ApplicationDirectory (or UserConfiguration)
Directory where to find custom configuration and automation objects.<br>
Example (which is the default value) :<br>
`ApplicationDirectory=/usr/local/etc/Majordome/`

# Automation Configuration

Automation configurations are grouped (for a specific Majordome instance) in a directory tree likes specified with the `ApplicationDirectory=` parameter. A typical directory tree looks like :
```
00_Majordome
01_Pool
02_LivingRoom
```

Directories, also known as *containers*, are loaded in order (00_* first, 01_* after, etc ...). As soon as declared, Majordome objects can be used in further ones.
An error is raised if an object is duplicated.

> [!TIP]
> Have a look on `00_Majordome` provided in [Documentations/SamplesCode/AllComponents/](Documentations/SamplesCode/AllComponents/) directory. It contains some code that can be usefull in all project. 
> ```
> LogToFile.lua
> MidnightOrAtLaunch.timer
> PurgeLog.lua
> README.md
> ```
> It defines **daily rotating log** in `/tmp` with automatic purging. See [Séléné](https://github.com/destroyedlolo/Selene)'s SelLog.

> [!CAUTION]
> If Majordome's objects (triggers, timers, ...) are shared, Lua scripts are **stateless** and Lua side variables, functions, can't be shared among scripts. See bellow about Lua scripting.

## Objects
Objects configuration is done using plain text files and the suffix determines their kind. Files without known suffix are ignored.

Supported are :
- [Timer](timer.md) (`.timer`) : specifies the absolute time or the interval to launch an action
- [Topic](tipic.md) (`.topic`) :  specifies MQTT topics. For incoming topic, the payload can be forced to be a numerical value and it can be stored as a SelShared variables with an optional expiration time (*time to live*, TTL). A *quality of service* (QoS) can be specified as well
- [Rendez-vous](rendezvous.md) (`.rendezvous`) : allows triggering an action without having to use an MQTT topic.
- [Tracker](tracker.md) (`.tracker`) : a simple but powerful state box to track some activities. Start an action at after an even, track its advancement, finishing it.
- last but not least, [Tasks](Task(lua).md)  (`.lua`) are Lua script that are triggered by events described above. Processing data, storing them, decision-making, triggering other objects ... they are the brain of the automation.

## Lua scripting

### Defined objects

Following variables are defined in launched scripts :
- **MAJORDOME_VERSION**
- **MAJORDOME_COPYRIGHT**
- **MAJORDOME_ClientID** - MQTT Client identifier, must be unique per broker.
- if compiled with DEBUG defined and if started with `-d` option, **MAJORDOME_DEBUG** is declared, otherwise, it is unset.

As well as following objects :
- all Séléné loaded modules (**SelLog**, **SelMQTT**, ...)
- **MQTTBroker** - Broker defined in Majordome's configuration file. This object aims to let Lua scripts to `Publish()` 

## Init.lua
In each subdirectory, `Init.lua` (with capital 'I') is executed at startup, during user configuration loading. It's mostly used to do initialization (so its filename :yum:).

# Examples

* [SamplesCode](SamplesCode/) - basic examples involving Majordome's components.
* [UseCases](UseCases/) - advanced examples

Time to time, I'll add additional examples or tutorial. In addition, `MaMaison` branch contains the code I'm using to control my house.
