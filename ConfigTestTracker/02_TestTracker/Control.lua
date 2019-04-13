-- This task will control the entire test
--
-- Indicate MQTT topic(s) to listen to
-->> listen=Control
--
-- Disable this script
--->> disabled

local tracker = MajordomeTracker.find("TestTracker")
if not tracker then
	print("Can't find ".. '"TestTracker"')
	return
end

if MAJORDOME_PAYLOAD == 'Status' then
	print 'Current tracker configuration :'
	print '-------------------------------'

	print("Tracker is coming from", tracker:getContainer())
	print("Tracker is", tracker:isEnabled() and 'Enabled' or 'Disabled')	print("Tracker's status is", tracker:getStatus())
end

