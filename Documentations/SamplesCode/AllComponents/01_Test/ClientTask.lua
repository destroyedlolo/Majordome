-- This is an example of a task script which will respond to a timer and a topic
-- In the header of the script (comment block at the very beginning of
-- the script), each line starting with -->> are Majordome's commands.
-- Consequently, '--->>' are commented out commands.
--
-- Name of this script
-- if not set, takes the filename
-- Notez-bien : if used, this directive MUST be defined before any
-- listen directive.
--->> name=Toto
--
-- Indicate MQTT topic(s) to listen to
-- (more than one "listen" can be present)
-->> listen=NoStations
--
-- Indicate the Timer(s) to wait for
-- (more than one "when" can be present)
-->> when=15s
--
-- Launched when a tracker is done
--->> whenDone=tracker
--
-- Launched when a tracker is started
--->> whenStarted=tracker
--
-- Launched when a tracker is stopped
--->> whenStopped=tracker
--
-- Wait for a rendez-vous (a.k.a event)
--->> waitfor=exemple
--
-- If set, only one instance is allowed to run
--->> once
--
-- remove some trace
-- This option is useful to avoid logging of very noisy topics
--->> quiet
--
-- Disable this script
--->> disabled

-- NOTEZ-BIEN : This command will be ignored as not part of the header
-->> Name : toto

-- Here starts the lua's code.
-- In case this task is launched by a MQTT trigger, following variables
-- are defined :
-- MAJORDOME_TOPIC_NAME variable contains the "name" of the topic that
-- triggered this task
-- MAJORDOME_TOPIC & MAJORDOME_PAYLOAD : the message

if not MAJORDOME_TIMER then
	print("It's a very useful Lua's script, triggered by Topic", MAJORDOME_TOPIC_NAME);
	print("Received information :", MAJORDOME_TOPIC, MAJORDOME_PAYLOAD)
else
	print("Launched by timer ", MAJORDOME_TIMER);
end
