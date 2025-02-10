Demonstrates usage of launching a [task](../../Task(lua).md)
by [topics](../../topic.md) API.

- **bip.lua** to be launched
- **master.lua** will `launch()` bip task.

---

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	UserConfiguration=Documentations/SamplesCode/Topic_Launch/
