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

Directories are loaded in order (00_* first, 01_* after, etc ...). As soon as declared, Majordome objects can be used in further ones. An error is raised if an object is duplicated.

**Notez-bien :** We are speaking here about Majordome's objects. Lua side variables can't be shared among scripts. See bellow about Lua scripting.
