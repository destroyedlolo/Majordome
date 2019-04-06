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

