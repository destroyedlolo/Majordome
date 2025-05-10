-- This task will control the entire test
--
-- Indicate MQTT topic(s) to listen to
-->> listen=ControlHM
--
-->> need_tracker=TestTrackerHM
--
-- Disable this script
--->> disabled

MAJORDOME_PAYLOAD = MAJORDOME_PAYLOAD:upper()

if MAJORDOME_PAYLOAD == 'STATUS' then
	print 'Current tracker configuration :'
	print '-------------------------------'

	print("Tracker is coming from", TestTrackerHM:getContainer())
	print("Tracker is", TestTrackerHM:isEnabled() and 'Enabled' or 'Disabled')
	print("Tracker's status is", TestTrackerHM:getStatus())
	print("Tracker's counter is", TestTrackerHM:getCounter())
elseif MAJORDOME_PAYLOAD == "CHECKING" or MAJORDOME_PAYLOAD == "START" then
	TestTrackerHM:setStatus( MAJORDOME_PAYLOAD )
end
