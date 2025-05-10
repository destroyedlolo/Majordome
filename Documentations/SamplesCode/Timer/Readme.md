Simple usage of a recurrent **timer**.

![Objects in this example](Diagram.svg)

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	ApplicationDirectory=Documentations/SamplesCode/Timer/

and the output looks like :

	2024-06-01 18:30:10 - *I* Application code for Majordome 4.000100 ...
	2024-06-01 18:30:10 - *L* Loading '01_CPUTemp'
	2024-06-01 18:30:10 - *I* Let's go ...
	2024-06-01 18:30:10 - *I* CPU temperature : 38.0°C
	2024-06-01 18:30:40 - *I* CPU temperature : 41.0°C
	2024-06-01 18:31:10 - *I* CPU temperature : 41.0°C
	2024-06-01 18:31:40 - *I* CPU temperature : 46.0°C
	2024-06-01 18:32:10 - *I* CPU temperature : 43.0°C
