# .rendezvous
A Rendez-vous triggers an action from another object or from Lua code. 
Typical usage is to synchronize automation within Majordome without having to use topics only for internal synchronisation.

## Directives

### name=
Unique name to identify the rendez-vous. If not set, uses the filename.
```
name=toto
```

### quiet
Remove some trace. Be less noisy

### disabled
This rendezvous starts as disabled.

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
## Lua exposed

Rendez-vous are exposed as `MajordomeRendezVous` with following methods :
- **Launch()** - Launchs associated tasks
- **getContainer()** - Returns rendez-vous container (the directory defining this rendez-vous)
- **getName()** - Rendez-vous' name
- **isEnabled()** - Is Rendez-vous enabled ?
- **Enable()** / **Disable()** as the function's name said
