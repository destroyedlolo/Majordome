A **tracker** is an object that follow a topic and trigger some actions depending of topic's value.

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

This is an example of usage of **howmany** : the tracker will wait for *how many* positive checks before becoming DONE.

Checking is based on tracker's script return code as following :
* **true** : increase "DONE" counter
* **false** : reset "DONE" counter
* other (including nil) : ignored

--------

This particular example will react to following topics :

* *Control* : control test behaviours
  * **STATUS** : displays the tracker status
  * **CHECKING** or **START** : the tracker checks for incoming values
* *Test* : value to be tested. If > 10, the tracker switch to *Done* state

