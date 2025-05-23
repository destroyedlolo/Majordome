-- Test objects creation as per needs_* directives.
--
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
--->> listen=NoStations
--
-- Create this topic object
-->> require_topic=Test
--
-- Indicate the Timer(s) to wait for
-- (more than one "when" can be present)
--->> when=15s
--
-- Create this timer object
-->> need_timer=25s
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
-- Create this tracker object
-->> need_tracker=Tracker
--
-- Wait for a rendez-vous (a.k.a event)
--->> waitfor=example
--
-->> need_rendezvous=example
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
--
-- Start at Majordome startup
-->> RunAtStartup

-- Test if the rendez-vous has been created
print( example )

-- Test if the tracker has been created
print( Tracker )

-- Test if the timer has been created
-- as '25s' is not a valid variable's name, we have to use _G
print( _G['25s'] )

-- Test if the topic has been created
print( Test )
