Test runifover feature. Will log a `bip` if running after 00:05

![Objects in this example](Diagram.svg)

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	ApplicationDirectory=Documentations/SamplesCode/TimerOver/

and the output looks like :

    2024-06-10 20:13:38 - *C* User configuration directory : 'Documentations/SamplesCode/TimerOver/'
    2024-06-10 20:13:38 - *I* Application code for Majordome 4.000600 ...
    2024-06-10 20:13:38 - *L* Loading '01_TImerOverNouveau dossier'
    2024-06-10 20:13:38 - *L* 	'Documentations/SamplesCode/TimerOver//01_TImerOverNouveau dossier/MidnightOrAtLaunch.timer'
    2024-06-10 20:13:38 - *C* 		Running at 0:5
    2024-06-10 20:13:38 - *C* 		Run if over
    2024-06-10 20:13:38 - *L* 	'Documentations/SamplesCode/TimerOver//01_TImerOverNouveau dossier/bip.lua'
    2024-06-10 20:13:38 - *C* 		Added to timer 'MidnightOrAtLaunch'
    2024-06-10 20:13:38 - *I* Application starting ...
    2024-06-10 20:13:38 - *C* Subscribing to 0 actives topics
    2024-06-10 20:13:38 - *I* Bip
