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

### -->> when=
Indicates the Timer to wait for : 
this script will be triggered when this timer is exhausted.
```
-->> listen=15s
```
If a task is woken up by a timer, the following variables are created at Lua side.
- **MAJORDOME_TIMER**, name of the timer (in the example above `15s`)
