# .timer
Define a timer for recurring tasks.
Its target can be an absolute time or a recurring interval. 

## Directives

> [!IMPORTANT]  
> As of v6.0 onward, The syntax has changed for **timers** and is not any more compatible with previous versions.

### General directives
### Generals
See [this page](Headers%20and%20Shared%20Directives.md#general-directives).

### Timer's
#### -->> every=
Specify repeating delay (in seconds), ignored if null.
```
-->> every=15
```
#### -->> at=
Set an absolute time to this timer using format format `HHMM`.
```
-->> at=2256
```
The trigger will be raised at 56 to 10 PM.

> [!NOTE]
> **every=** has precedence on **-->> at=**, if both are present, only **-->> every=** is took in account.

#### -->> immediate
Trigger this timer at startup. If not set, it will trigger after the initial delay.

#### -->> runifover
If Majordome is starting after `at=` time, on the same day, this timer is trigger.

## Example
File **MidnightOrAtLaunch.timer**
```
# Timer to trigger an action at midnight or when Majordome is launched

# Launch it at midnight
-->> at=0000

# or at launch time
-->> runifover
```

## at Lua side
### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current timer

### Exposed objects
Timers are exposed as **MajordomeTimer** with following methods :
- `getContainer()` returns the container (directory) in which this timer as been defined
- `getName()` returns timer's name
- `isEnabled()` returns a boolean reflecting if the timer is enabled or not
- `Enable()` to enable this timer
- `Disable()` to disable this timer
- `Launch()` to launch all attached handlers
- `getEvery()` and `setEvery()` to get or change *every* timer
- `getAt()` and `setAt()` to get or change *at* timer in the form **hh.mm**
- `getAtHM(h, m)` and `h,m = getAtHM()` to get or change *at* timer in with its 2 components, hour and minute
- `Reset()` to reset an *every* timer
