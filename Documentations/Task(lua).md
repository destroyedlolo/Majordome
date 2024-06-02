# .lua
**Tasks** are Lua scripts that are triggered by external events (timer, rendezvous, topic ...). They are the core of the automation in Majordome.

⚠️ Notez-bien ⚠️ : each task is running is a **dedicated context** (i.e, different Lua state) and, consequently, are **strongly stateless**.
You should assume that the environment exists only for a single invocation and will disappear as soon as the task is finished. The script should initialize any needed objects when it is starting and should commit any permanent data changes before exiting to a durable store such Séléné's shared (`SelSharedVar`, `SelSharedFunction`, Collections, ...) or publish them to the MQTT network. Variables/functions/whatever can't be shared among scripts. Again, Séléné is providing some sharing mechanisms if needed.

For advanced/complex usages, you may serialize data needed to be propagated within a SelSharedVar, or, smarter, to an MQTT message. MQTT context sharing open the door to distributed processing, horizontal scalability, load balancing and fault resilience.

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

### -->> whenChanged=
The script is launched when a tracker status is changed.
```
-->> whenChanged=tracker
```

### -->> once
Only one instance is allowed to run at the same time : no concurrency.

### --> quiet
Removes some trace.

### -->> disabled
This script won't run.

## at Lua side

### Exposed variables
As described above, following variables are subject to be created :
- **MAJORDOME_TOPIC_NAME**, **MAJORDOME_TOPIC** and **MAJORDOME_PAYLOAD** if the task is launched by an MQTT topic.
- **MAJORDOME_TIMER** if the task is launched by a timer
- **MAJORDOME_TRACKER** and  **MAJORDOME_TRACKER_STATUS** if the task is launched by a  tracker.

Following variables are also created :
- **MAJORDOME_VERSION** and **MAJORDOME_COPYRIGHT** as the name said
- **MAJORDOME_ClientID**, MQTT client identifier that must be unique for a single broker.
- **MAJORDOME_DEBUG**, set only if Majordome has been started with `-d` and, consequently, is in debugging mode.

### Exposed objects
- **MQTTBroker**, the master broker as defined in the configuration file.

As well as following objects kinds :
- **MajordomeTask**, tasks' interface which implements
  - `Launch()` Launchs/executes this task
  - `getContainer()` returns the container (directory) in which this task as been defined
  - `getName()` returns task's name
  - `isEnabled()` returns a boolean reflecting if the task is enabled or not
  - `Enable()` to enable this task
  - `Disable()` to disable this task

- **MajordomeMQTTTopic**, topic's interface as defined with `.topic` file
- **MajordomeTimer**, timer's interface
- **MajordomeTracker**, tracker's interface

Each kind implement a `find()` method to retreive an object by its name.

See dedicated documentation for more information.
