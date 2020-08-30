A **tracker** is an object that follow a topic and trigger some actions depending of its value.

Typical example : Follow a room temperature to close its shutter if too hot.
Status are :

	WAITING : incoming value are ignored.
	 ||
	---- Start time
	 ||
	CHECKING : looking for data
	 ||
	(lua) Is value above limite ?
		-> Yes : DONE : incoming value are ignored.
	 ||
	---- Stop time
	 ||
	WAITING

In this example, the decision is propagated using tracker's getStatus()

Alternatively, the status can be changed by the tracker's script return code.
* Return code = **true** - change to *Done* status
* other value - don't change the status **ONLY IF *howmany* UNSET or =1**

--------

This particular example will react to following topics :

* *Control* : control test behaviours
  * **STATUS** : displays the tracker status
  * **MANUAL** : disables timers so the tracker stays in the same state
  * **AUTO** : activates timers so the tracker switches automatically b/w waiting and checking state
  * **WAITING** or **STOP** : the tracker waits for a start signal
  * **CHECKING** or **START** : the tracker checks for incoming values
  * **DONE** : expected value found
* *Test* : value to be tested. If > 10, the tracker switch to *Done* state
