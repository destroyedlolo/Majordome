# .lua
**Tasks** are Lua scripts that are triggered by external events (timer, rendezvous, topic ...). They are the core of the automation in Majordome.

> [!CAUTION]
> ⚠️ Notez-bien ⚠️ : each task is running is a **dedicated context** (i.e, different Lua state) and, consequently, are **strongly stateless**.<br>
> You should assume that the environment exists only for a single invocation and will disappear as soon as the task is finished. The script should initialize any needed objects when it is starting and should commit any permanent data changes before exiting to a durable store such Séléné's shared (`SelSharedVar`, `SelSharedFunction`, Collections, ...) or publish them to the MQTT network. Variables/functions/whatever can't be shared among scripts. Again, Séléné is providing some sharing mechanisms if needed.

> [!TIP]
> For advanced/complex usages, you may serialize data needed to be propagated within a SelSharedVar, or, smarter, to an MQTT message. MQTT context sharing open the door to distributed processing, horizontal scalability, load balancing and fault resilience.

## Directives
In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` (2 dashes) are Majordome's directives.
If you want to comment out a directive, use `--->>` (3 dashes)
### General directives
#### -->> name=
Unique name to identify the topic. If not set, uses the filename.
```
-->> name=Toto
```
#### --> quiet
Removes some trace.

#### -->> disabled
This script won't run.

### Task's
#### -->> once
Only one instance is allowed to run at the same time : no concurrency.

### Triggering
Following directives determine what will trigger this script.<br>
Multiple directives may be present, including those of the same kind.

#### -->> RunAtStartup
Run when Majordome starts.
> [!IMPORTANT]  
> Startup tasks run **before** any other actions, i.e., before topics are subscribed, timers started, ...
> On the other hand, [**Timer**](timer.md)'s immediate functions are launched after everything is configured.

#### -->> listen=
Indicates [**MQTT topic**](topic.md) to listen to : this script will be launched when a data
is received on this topic.
```
-->> listen=NoStations
```
If a task is woken up by an MQTT topic, the following variables are created at Lua side :
- **MAJORDOME_TOPIC_NAME**, name of the topic (in the example above `NoStations`)
- **MAJORDOME_TOPIC**, the MQTT topic itself
- **MAJORDOME_PAYLOAD**, message's payload.

#### -->> waitfor=
Indicate the [**Rendezvous**](rendezvous.md) to wait for.
```
-->> waitfor=exemple
```

#### -->> when=
Indicates the [**Timer**](timer.md) to wait for : 
this script will be triggered when this timer is exhausted.
```
-->> when=15s
```
If a task is woken up by a timer, the following variables are created at Lua side.
- **MAJORDOME_TIMER**, name of the timer (in the example above `15s`)

#### -->> whenStarted=
The script is launched when provided [**tracker**](tracker.md) is started (beggin *following* mode).
```
-->> whenStarted=tracker
```
**MAJORDOME_TRACKER** is created with tracker's name and **MAJORDOME_TRACKER_STATUS** with its condition.

#### -->> whenDone=
The script is launched when provided [**tracker**](tracker.md) is done (existing *following* mode cause the condition is met).
```
-->> whenStarted=tracker
```

#### -->> whenStopped=
The script is launched when a [**tracker**](tracker.md) is stopped.
```
-->> whenStopped=tracker
```

#### -->> whenChanged=
The script is launched when a [**tracker**](tracker.md) status is changed.
```
-->> whenChanged=tracker
```
### Dependancies
Tasks usually depends on Majordome's objects ; those directives expose them at Lua level without having to manually `find()` them.

> [!WARNING]
> Only `store`d topic can be  needed or required.

#### -->> need_topic=
Creates an object if a value has been received and its value hasn't expired. Otherwise, the object remains unset.

#### -->> require_topic=
Prevents the script to be launched if corresponding value is not valid.<br>

#### -->> need_rendezvous=, -->> need_tracker=, -->> need_timer=, -->> need_task=, -->> need_minmax, -->> need_namedminmax, -->> need_shutdown
Create corresponding object.

## at Lua side

### Exposed variables
#### Majordom's variables
Following variables are created :
- **MAJORDOME_VERSION** and **MAJORDOME_COPYRIGHT** as the name said
- **MAJORDOME_CONFIGURATION_DIRECTORY**, the root of the configuration directory
- **MAJORDOME_ClientID**, MQTT client identifier that must be unique for a single broker.
- **MAJORDOME_DEBUG**, set only if Majordome has been started with `-d` and, consequently, is in debugging mode.

- **MAJORDOME_Myself** is automatically created and correspond to the current task

#### Who launched this task ?
Following variables are created depending who triggering this task
- **MAJORDOME_TOPIC_NAME**, **MAJORDOME_TOPIC** and **MAJORDOME_PAYLOAD** if the task is launched by an [MQTT topic](topic.md).
- **MAJORDOME_TIMER** : name of the triggering [timer](timer.md)
- **MAJORDOME_TRACKER** and  **MAJORDOME_TRACKER_STATUS** if the task is launched by a  tracker.

### Exposed objects
- **MQTTBroker**, the master broker as defined in the configuration file.

As well as following objects kinds :
- **MajordomeTask**, tasks' interface which implements
  - `Launch()` Launchs/executes this task in the same thread. Therefore, Lua variables/functions can be shared between those scripts. But don't forget that these shares will be lost if the called task executes following an event.
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
