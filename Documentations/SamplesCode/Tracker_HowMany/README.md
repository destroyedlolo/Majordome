This is an example of usage of **howmany** : the tracker will wait for *how many* positive checks before becoming DONE.

Following objects are created :
- `TestHM.topic` - subscribes to a topic **TestHM** to receive data
- `TestTrackerHM.tracker` - A tracker waiting for 3 incoming data > 10

<!---
- `ControlHM.topic` - subscribes to a topic **ControlHM** to send command to the tracker. Recognized commands are :

  * **STATUS** : displays the tracker status
  * **CHECKING** or **START** : Force the status of the tracker for testing purposes
--->

---

To test it, create a config file like this :

	# URL to reach the broker
	Broker_URL=tcp://torchwood.local:1883

	# Application directory
	ApplicationDirectory=Documentations/SamplesCode/Tracker_HowMany

