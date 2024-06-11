Demonstrates usage of `require_topic` directives with a [task](../../Task(lua).md)
launched by another task.

- **bip.lua** is a task that `require_topic=` **Test.topic**.
- **master.lua** will `launch()` bip task with **Test** unset then set.

---

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	UserConfiguration=Documentations/SamplesCode/Tasks_Launch/
