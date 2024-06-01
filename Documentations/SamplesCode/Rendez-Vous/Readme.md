Demonstrate useage of rendez-vous.

**Startup.lua** will be launched at Majordome's startup and will launch **Result.lua** task through **example** rendez-vous.

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	UserConfiguration=Documentations/SamplesCode/Rendez-Vous/

and the output looks like :

	2024-06-01 17:20:23 - *I* Application code for Majordome 4.000000 ...
	2024-06-01 17:20:23 - *L* Loading '01_Sample_Rendez-Vous'
	2024-06-01 17:20:23 - *I* Let's go ...
	2024-06-01 17:20:23 - *I* Rendez-vous found, lets launch it
	2024-06-01 17:20:23 - *I* Result is running
	
