-- This is an example of a task script
-- In the header of the script (comment bloc at the very beginning of
-- the script), each line starting with -->> are Majordome's commands.
-- Consequently, '--->>' are commented out commands.
--
-- Name of this script
-- if not set, takes the filename
-- Notez-bien : if used, this directive MUST be defined before any
-- listen directive.
--->> name=Toto
--
-- Indicate topic to listen for
-- (more than one "listen" can be present)
-->> listen=NoStations
--
-- If set, only one instance is allowed to run
-->> once
--
-- Disable this script
--->> disabled

-- NOTEZ-BIEN : This command will be ignored as not part of the header
-->> Name : toto

	print "It's a very useful Lua's script"
