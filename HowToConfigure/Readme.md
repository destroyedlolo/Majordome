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

# General configuration file

A configuration file instructs Majordome about its basic configuration and where are located automation objects.<br>
Lines starting by a hash sign `#` are considered as comment and, as such, ignored.

:sunglasses: Thanks to `-f`, it's possible to specify the configuration file to use, consequently, it's possible to have several instances running with totally separate configuration.

## Known configuration directives
### Broker_URL
**MQTT Broker** to connect to. With *Paho* syntax that is : `tcp://*fqdn_hostname*:*port*`.<br>
Example (which is the default value) :<br>
`Broker_URL=tcp://localhost:1883`

### ClientID
**Client ID** to uniquely identify to the broker.<br> In case you have more than one Majordome instance connected to a single broker, you **MUST** set a unique ID per instance.

If not set, the Client ID will be `Majordome-*Hostname*-*pid*`.

Example :<br>
`ClientID=Majordome`

### UserConfiguration
Directory where to find custom configuration and automation objects.<br>
Example (which is the default value) :<br>
``UserConfiguration=/usr/local/etc/Majordome/``

# Automation Configuration

Automation configurations are grouped (for a specific Majordome instance) in a directory tree likes :
```
00_Majordome
01_Pool
02_LivingRoom
```

Directories are loaded in order (00_* first, 01_* after, etc ...). As soon as declared, Majordome objects can be used in further ones.<br>
:exclamation: **Notez-bien** :exclamation: Majordome's objects (triggers, timers, ...) are shared. But Lua side variables can't be shared among scripts. See bellow about Lua scripting.

An error is raised if an object is duplicated.

Have a look on `00_Majordome` provided in **Config** directory : 
```
LogToFile.lua
MidnightOrAtLaunch.timer
PurgeLog.lua
README.md
```
It defines **daily rotating log** in `/tmp` with automatic purging. See [Séléné](https://github.com/destroyedlolo/Selene)'s SelLog.

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

## Objects
Files without known suffix are ignored.

---

### Init.lua
In each subdirectory, `Init.lua` (with capital 'I') is executed at startup, during user configuration loading. It's mostly used to do initialization (so its filename :yum:).

---

