# .tracker
Trackers are a simple state box to track some activities and trigger some actions depending on conditions set in its Lua own script.

As an example, temperature followup to close shutters in order to prevent over heating of room directly exposed to the sun :
- temperature followup is starting at a fixed time (i.e., not needed before 10:00 as the sun is too low), and end up to another one (let's say, after 17:00, it's too late to close shutters).
- during this time window, we are checking if the temperature goes above our target and in such case, shutters are closed and there is no need to continue the followup.

Trackers make easy this kind of simple automation.

Typical example : Follow a room temperature to close its shutter if too hot.
Status are :

	Status 'WAITING' : incoming value are ignored.
	 ||
	---- Start time
	 ||
	Status 'CHECKING' : looking for data
	 ||
	(lua) Is value above limite ?
		-> Yes : new status 'DONE' : incoming value are ignored.
	 ||
	---- Stop time
	 ||
	Status 'WAITING' : the racker is reseted.

The status can **MajordomeTracker** API or be changed by the tracker's script return code.
* Return code = **true** - decreases *howmany* counter and propotes status to *DONE* if the counter is over (by default, this counter is set to **1** meaning the 1st positive result is promoted to *DONE*).
* Return code = **false** - keep in *CHECKING* status but the howmany counter is reset
* no return code - keep the tracker as it is and *howmany* remains untouched

## Syntax

Trackers are basically **[Lua tasks](Task(lua).md)** and are following the same syntax.

In the header of the script (comment block at the very beginning of the script), each line starting with `-->>` are Majordome's commands.<br>
Consequently, `--->>` are commented out commands (notice the 3 dashes).

## Directives

### Generals

#### -->> name=
Unique name to identify the tracker. If not set, uses the filename.
```
-->> name=toto
```
#### -->> quiet
Remove some trace. This option is useful to avoid logging of very noisy topics.

#### -->> disabled
This tracker starts as disabled : stats change and incoming messages are ignored.

### Tracker owns

#### -->> howmany=
Numbers of consecutive positive responses before the tracker status become DONE.

#### -->> activated
The tracker will start in **CHECKING** mode

### Timer to start/stop the tracker

#### -->> start=
Timer indicating when the tracker will switch to **CHECKING** mode

#### -->> stop=
Timer indicating when the tracker will be reseted to **WAITING** mode

### MQTT topics

#### -->> listen=
Indicates which [Topic](topic.md) to follow to take a decision.<br>
(more than one "listen" can be present)

#### -->> statustopic=
Report status change to the given topic.
Notez-bien : it's not a [Majordome topic](topic.md) but a simple string where `%ClientID%` will be replaced by Majordome's MQTT client ID.

### Dependancies
Tasks usually depends on Majordome's objects ; those directives expose them at Lua level without having to manually `find()` them.

> [!WARNING]
> Only `store`d topic can be  needed or required.

#### -->> need_topic=
Creates an object if a value has been received and its value hasn't expired. Otherwise, the object remains unset.

#### -->> require_topic=
Prevents the script to be launched if corresponding value is not valid.<br>

#### -->> need_rendezvous=, -->> need_tracker=, -->> need_timer=, -->> need_task=
Create corresponding object.

## at Lua side
### Exposed variables
- **MAJORDOME_Myself** is automatically created and correspond to the current tracker
- **MAJORDOME_TRACKER** - tracker's name
- **MAJORDOME_TRACKER_STATUS** - tracker's status

### Exposed objects
Trakers are exposed as **MajordomeTracker** with following methods :
- `getContainer()` returns the container (directory) in which this tracker as been defined
- `getName()` returns tracker's name
- `isEnabled()` returns a boolean reflecting if the tracker is enabled or not
- `Enable()` to enable this tracker
- `Disable()` to disable this tracker 
- `getCounter()` get the *HowMany* counter (counting downward)
- `resetCounter()` resert the *HowMany* counter
- `getStatus()` get current tracker status
- `setStatus()` set the current tracker status. Argument should be **CHECKING** (or **START**), **WAITING** (or **STOP**) and any other value leads to **DONE** status.
