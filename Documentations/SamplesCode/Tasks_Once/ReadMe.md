Demonstrates usage of `Once` directives with [tasks](../../Task(lua).md).

The task is triggered every 15s, but, thanks to **Once** directive, 
it can't be relaunched until the 20s waiting time is over. Consequently, only one instance 
of *Run For 20s* can run at once.

It's also demonstrates how to load additionnal mobules from `init.lua`.

> [!CAUTION]
> It is very very not recommended to use Selene.Sleep() (or any SelScripting 
> methods) in Majordome tasks as it's the case in this *example*.

---

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	UserConfiguration=Documentations/SamplesCode/Tasks_Once/


