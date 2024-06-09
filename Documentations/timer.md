# .timer
Define a timer for recurring tasks.
Its target can be an absolute time or a recurring interval. 

## Directives

### name=
Unique name to identify the topic. If not set, uses the filename.
```
name=toto
```

### every=
Specify repeating delay (in seconds), ignored if null.
```
every=15
```

### at=
Set an absolute time to this timer using format format `HHMM`.
```
at=2256
```
The trigger will be raised at 56 to 10 PM.

> [!NOTE]
> **every=** has precedence on **at=**, if both are present, only **every=** is took in account.

### immediate
Trigger this timer at startup. If not set, it will trigger after the initial delay.

### runifover
If Majordome is starting after `at=` time, on the same day, this timer is trigger.

### quiet
Silences some logs. This option is useful to avoid logging of very noisy topics.

### disabled
This trigger starts as disabled : incoming messages are ignored.

## Example
File **MidnightOrAtLaunch.timer**
```
# Timer to trigger an action at midnight or when Majordome is launched

# Launch it at midnight
at=0000

# or at launch time
runifover
```
