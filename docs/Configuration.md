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

Automation configuration are grouped (for a specific Majordome instance) in a directory tree likes :
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

## Objects
Files without known suffix are ignored.

---

### Init.lua
In each subdirectory, `Init.lua` (with capital 'I') is executed at startup. It's mostly used to do initialization (so its filename :yum:).

---

### .topic : MQTT topic definition
Define **MQTT topic**, with following known parameters :
#### name=
Unique name to identify the topic. If not set, takes the filename.
```
name=toto
```
#### topic=
Topic to listen to. This field is mandatory.
```
topic=Temperature/Pool
```

#### qos=
Quality of service for this topic between **0** and **2**. If outside this range, forced to **0** (which is the default value)
```
qos=2
```
#### store
Store received payload in a SelShared variable with the name of this topic. As example :
```
name=titi
topic=example
store
```
If we receive a value "*toto*" in topic `example`, a SelShared variable "**titi**" is created with value "*toto*".
#### numeric
Store the payload as a numeric value (float).
#### ttl=
Time to live of this SelShared (in seconds). When the topic doesn't receive fresh data during this ttl period, SelShared is unset.
```
ttl=2.5
```
#### default=
Set the default value for this topic. This field must be defined after **name**, **numeric** and **ttl** and is useful only with **store**d topics.
```
default=3.14
```
#### disabled
This topic starts as disabled : incoming messages are ignored.

---

### .timer
Define a timer for recurring task.

#### name=
Unique name to identify the topic. If not set, takes the filename.
```
name=toto
```

#### every=
Specify repeating delay (in seconds), ignored if null.
```
every=15
```

#### at=
Set an absolute time to this timer using format format `HHMM`.
```
at=2256
```
The trigger will be raised at 56 to 10 PM.

:exclamation: **Notez-bien** :exclamation: **every=** has precedence on **at=**, if both are present, only **every=** is took in account.

#### at=
