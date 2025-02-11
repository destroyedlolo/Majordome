# .rendezvous
A Rendez-vous triggers an action from another object or from Lua code. 
Typical usage is to synchronize automation within Majordome without having to use intermediate topic.

## Directives

> [!IMPORTANT]  
> As of v6.0 onward, The syntax has changed for **Rendezvous** and is not any more compatible with previous versions.

### General directives
#### -->> name=
Unique name to identify the rendez-vous. If not set, uses the filename.
```
-->> name=Toto
```
#### --> quiet
Removes some trace.

#### -->> disabled
This rendez-vous will not trigger any handler.

## Example
File **test.rendezvous**
```
-- Example of a "Rendez-vous"
-- 
-- Directives starting with a '-->>' are took in account.
-- Ones with a '--->>' are commented out
--
-- name of the event
-- if not set, takes the filename
--->> name=toto
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet
--
-- disable this event
--->> disabled
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

## at Lua side
### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current tracker

### Exposed objects
RendezVous are exposed as **MajordomeRendezVous** with following methods :
- `getContainer()` returns the container (directory) in which this tracker as been defined
- `getName()` returns tracker's name
- `isEnabled()` returns a boolean reflecting if the tracker is enabled or not
- `Enable()` to enable this tracker
- `Disable()` to disable this tracker
- `Launch()` to launch all attached handlers
