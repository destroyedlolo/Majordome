Demonstrate usage of **Resources**.

![Objects in this example](Diagram.svg)

**Startup.lua** will be launched at Majordome's startup and will launch **Result.lua** task through **example** rendez-vous.

To test it, create a config file like this

	# URL to reach the broker
	Broker_URL=tcp://<your_broker>:1883

	# Application directory
	UserConfiguration=Documentations/SamplesCode/Resource/

and the output looks like :


	*D* SeleneCore found : version 8
	2025-10-04 19:28:41 - *D* SelLog found : version 5
	2025-10-04 19:28:41 - *D* SelLua found : version 11
	2025-10-04 19:28:41 - *D* SelElasticStorage found : version 3
	2025-10-04 19:28:41 - *D* SelMultitasking found : version 1
	2025-10-04 19:28:41 - *D* SelSharedVar found : version 1
	2025-10-04 19:28:41 - *D* SelMQTT found : version 3
	2025-10-04 19:28:42 - *I* Application code for Majordome 6.130200 ...
	2025-10-04 19:28:42 - *L* Loading '50_Resource'
	2025-10-04 19:28:42 - *I* Application starting ...
	2025-10-04 19:28:42 - *E* ['Test'] ERROR:  relation "Test" does not exist
	LINE 1: INSERT INTO "Test" SELECT DATE_TRUNC('Day', sample_time) AS ...
                    ^

	2025-10-04 19:28:42 - *E* ['Test'] ERROR:  relation "Test" does not exist
	LINE 1: DELETE FROM "Test" WHERE sample_time::date <= current_date -...
                    ^

	2025-10-04 19:28:42 - *2* 2nd task starting
	2025-10-04 19:28:45 - *2* 2nd task finished
	2025-10-04 19:28:45 - *T* Task 'Task2Fail' from 'Documentations/SamplesCode/Resource/50_Resource' prevented to run by a busy resource
	2025-10-04 19:28:45 - *1* 1st task starting
	2025-10-04 19:28:48 - *1* 1st task finished

> [!NOTE]
> SQL errors are expected as nothing as been done at database side to create a working environment.
> This code only demonstrates the resource is took in account

- **Purge**, **archiving**, **FirstTask** and **SecondTask** will run in a (random) sequence as restricted by **once.resource**
- **Task2Fail** will ... fail as prevented to run if the resource is not free.
