# Headers and Shared directives

This page describes :
- how directives are declared in object's header
- which directives are shared among objects

## Definition

- an **event**  is an object that handle a particular condition to triggers its handlers. Example of event can be 
  - MQTT message arriving (see [topics](topic.md)). In such case, a data is provided to handlers, the content of the message
  - when its time to do something (see [timers](timer.md)). In such case, obviously, no data is provided.

- a **Handler** is an object that will be triggered when an event happens. It can be a [Lua Task](Task(lua).md), database [feeding](Database/feed.md), ...

> [!TIP]
> If the handler can execute Lua code, environment variables help to determine what triggered.
 
## Headers

All Majordome's configuration files starts with a header that contains 
**comments** and **Majordome directives**, as follows :

```Lua=
01 --My configuration file
02 --
03 -- A comment line
04 --
05 -->> Enabled Majordome directive
06 --->> Disabled Majordome directive
07 --
08 -- here after a blank line
09
10 -- Now, optional Lua code
11 -->> not anymore a Majordome directive, 
12 -- we're not anymore in the header
```

Heach header's line starts with a double dash `--`.

The first line without these double dash (as line **9**) means that we're not anymore in the header, potentially in a Lua code.

Each line starting with `-->>` (line **5**) are Majordome's directives.<br>
If you want to comment out a directive, use `--->>`, notice the 3 dashes (line **6**).

## General directives
The following directives are applicable to all Majordome's objects.

### -->> name=
Unique name to identify the object. If not set, uses the filename.
```
-->> name=Toto
```
> [!CAUTION]
> When doing **D2 diagram generation**, `name=` has to be placed before any other directive, otherwise the diagram will be probably wrong.

### -->> quiet
Removes some trace.

### -->> disabled
This object is disabled : 
- if a script is not be executed
- if a MQTTTopic, the incoming messages will be ignored
- if a timer, it will not trigger children

### -->> desc=
Add a description to the object.<br>
This field is useful to generate a small description in automatic generated diagrams (using **D2**, see [Majordome **-2**](Readme.md) option).

## Triggering while providing data

Those directives trigger handlers and provide a data.

### -->> listen=
Indicates [**MQTT topic**](topic.md) to listen to : this script will be launched when a data
is received on this topic.
```
-->> listen=NoStations
```
If a task is woken up by an MQTT topic, the following variables are created at Lua side :
- **MAJORDOME_TOPIC_NAME**, name of the topic (in the example above `NoStations`)
- **MAJORDOME_TOPIC**, the MQTT topic itself
- **MAJORDOME_PAYLOAD**, message's payload.

## Triggering without data

### -->> waitfor=
Indicate the [**Rendezvous**](rendezvous.md) to wait for.
```
-->> waitfor=exemple
```

### -->> when=
Indicates the [**Timer**](timer.md) to wait for : 
this script will be triggered when this timer is exhausted.
```
-->> when=15s
```
If a task is woken up by a timer, the following variables are created at Lua side.
- **MAJORDOME_TIMER**, name of the timer (in the example above `15s`)

### -->> whenStarted=
The script is launched when provided [**tracker**](tracker.md) is started (beggin *following* mode).
```
-->> whenStarted=tracker
```
**MAJORDOME_TRACKER** is created with tracker's name and **MAJORDOME_TRACKER_STATUS** with its condition.

### -->> whenDone=
The script is launched when provided [**tracker**](tracker.md) is done (existing *following* mode cause the condition is met).
```
-->> whenDone=tracker
```

### -->> whenStopped=
The script is launched when a [**tracker**](tracker.md) is stopped.
```
-->> whenStopped=tracker
```

### -->> whenChanged=
The script is launched when a [**tracker**](tracker.md) status is changed.
```
-->> whenChanged=tracker
```
## Dependancies
### need_
Handlers usually depend on other objects ; those directives expose them at Lua side without having to manually `find()` them.
> [!TIP]
> With V6 and below, needed may fail while looking for same priority objects (i.e : if a **tracker** needs a **minmax** ...). In such case, use the *old way* by using find() function.
> Here an example :
```lua
local tracker = MajordomeTracker.find("TestTracker")
if not tracker then
	print("Can't find ".. '"TestTracker"')
	return
end

local t15s = MajordomeTimer.find("15s")
local t25s = MajordomeTimer.find("25s")
if not t15s or not t25s then
	SelLog.Log('E',"Can't find '15s' or '25s' timer")
	return
end
```

#### Topics
##### -->> need_topic=
Creates an object if a value has been received and its value hasn't expired. Otherwise, the object remains unset.

#### Others
##### -->> need_rendezvous=, -->> need_tracker=, -->> need_timer=, -->> need_task=, -->> need_minmax, -->> need_namedminmax, -->> need_shutdown
Create corresponding object.

#### With Toile extension
##### -->> need_renderer
Create corresponding object (Only Toile plug-in has been compiled).

### require_
A handler will be triggered **only** if all these require are assumed.<br>
As an example, `-->> require_topic=Test` imply a message has been received and it hasn't been discarded by a timeout.

#### Topics
> [!WARNING]
> Only `store`d topic can be  needed or required.

##### -->> require_topic=
Prevents the script to be launched if corresponding value is not valid.
