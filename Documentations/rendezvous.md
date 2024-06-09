# .rendezvous
A Rendez-vous triggers an action from another object or from Lua code. 
Typical usage is to synchronize automation within Majordome without having to use intermediate topic.

## Directives

### name=
Unique name to identify the rendez-vous. If not set, uses the filename.
```
name=toto
```

### quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

### disabled
This topic starts as disabled : incoming messages are ignored.

## Example
File **test.rendezvous**
```
# name of the topic
# Here, commented out, so we take the filename, "test"
# name=

# Don't log incomming messages on this topic
#quiet

# disable this topic
#disabled
```

File **trigger.lua**
```
-- This script starts at Majordome startup and trigger
-- our rendez-vous.
--
-->> RunAtStartup

-- Find out our rendez-vous by its name
local rdv =  MajordomeRendezVous.find("test")

-- check if it's found
assert(rdv, "Can't find rendez-vous")

-- Launch it
SelLog.Log("Rendez-vous found, lets launch it")
rdv:Launch()
```

File **Result.lua**
```
-- Task to be launched by the rendez-vous.
--
-->> waitfor=test

SelLog.Log("Result is running")
```
