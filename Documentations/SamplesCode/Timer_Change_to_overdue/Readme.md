Test runifover feature. Will log a `bip` if running after 00:05

![Objects in this example](Diagram.svg)

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	ApplicationDirectory=Documentations/SamplesCode/Timer_Change_to_overdue

and the output looks like :

```
2025-02-11 14:56:36 - *I* Application code for Majordome 6.001000 ...
2025-02-11 14:56:36 - *L* Loading '50_TimerChangeOverdue'
2025-02-11 14:56:36 - *I* Application starting ...
It's currently :	14:56
Changing timer's target
Bip : I'm launched by the timer !
```
