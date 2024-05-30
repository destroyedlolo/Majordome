# .lua
**Tasks** are Lua scripts that are triggered by external events (timer, rendezvous, topic ...). They are the core of the automation in Majordome.

## Directives
In the header of the script (comment block at the very beginning of the script), each line starting with -->> (2 dashes) are Majordome's directives.
If you want to comment out a directive, use '--->>' (3 dashes)

### -->> name=
Unique name to identify the topic. If not set, uses the filename.
```
-->> name=Toto
```

### -->> listen=
Indicates MQTT topic(s) to listen to : this script will be triggered when a data
is received on this topic.
```
-->> listen=NoStations
```
If a task is woken up by an MQTT topic, the following variables are created at Lua side :
- **MAJORDOME_TOPIC_NAME**, name of the topic (in the example above `NoStations`)
- **MAJORDOME_TOPIC**, the MQTT topic itself
- **MAJORDOME_PAYLOAD**, message's payload.

More than one "listen=" can be present.

### -->> waitfor=
Indicate the rendezvous to wait for.
```
-->> waitfor=exemple
```

### -->> when=
Indicates the Timer to wait for : 
this script will be triggered when this timer is exhausted.
```
-->> when=15s
```
If a task is woken up by a timer, the following variables are created at Lua side.
- **MAJORDOME_TIMER**, name of the timer (in the example above `15s`)

### -->> whenStarted=
The script is launched when provided tracker is started (beggin *following* mode).
```
-->> whenStarted=tracker
```
**MAJORDOME_TRACKER** is created with tracker's name and **MAJORDOME_TRACKER_STATUS** with its condition.

### -->> whenDone=
The script is launched when provided tracker is done (existing *following* mode cause the condition is met).
```
-->> whenStarted=tracker
```

### -->> whenStopped=
The script is launched when a tracker is stopped.
```
-->> whenStopped=tracker
```

### -->> once
Only one instance is allowed to run at the same time : no concurrency.

### --> quiet
Removes some trace.

### -->> disabled
This script won't run.


