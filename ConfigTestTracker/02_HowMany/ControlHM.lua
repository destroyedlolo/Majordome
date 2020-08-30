-- This task will control the entire test
--
-- Indicate MQTT topic(s) to listen to
-->> listen=ControlHM
--
-- Disable this script
--->> disabled

local tracker = MajordomeTracker.find("TestTrackerHM")
if not tracker then
	print("Can't find ".. '"TestTrackerHM"')
	return
end

MAJORDOME_PAYLOAD = MAJORDOME_PAYLOAD:upper()

if MAJORDOME_PAYLOAD == 'STATUS' then
	print 'Current tracker configuration :'
	print '-------------------------------'

	print("Tracker is coming from", tracker:getContainer())
	print("Tracker is", tracker:isEnabled() and 'Enabled' or 'Disabled')
	print("Tracker's status is", tracker:getStatus())
	print("Tracker's counter is", tracker:getCounter())
elseif MAJORDOME_PAYLOAD == "CHECKING" or MAJORDOME_PAYLOAD == "START" then
	tracker:setStatus( MAJORDOME_PAYLOAD )
end
