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

local t15s = MajordomeTimer.find("15s")
local t25s = MajordomeTimer.find("25s")
if not t15s or not t25s then
	SelLog.log('E',"Can't find '15s' or '25s' timer")
	return
end

MAJORDOME_PAYLOAD = MAJORDOME_PAYLOAD:upper()

if MAJORDOME_PAYLOAD == 'STATUS' then
	print 'Current tracker configuration :'
	print '-------------------------------'

	print("Tracker is coming from", tracker:getContainer())
	print("Tracker is", tracker:isEnabled() and 'Enabled' or 'Disabled')
	print("Tracker's status is", tracker:getStatus())
	print("15s is", t15s:isEnabled() and 'Enabled' or 'Disabled')
	print("25s is", t25s:isEnabled() and 'Enabled' or 'Disabled')
elseif MAJORDOME_PAYLOAD == 'MANUAL' then	-- Disable control timers
	t15s:Disable()
	t25s:Disable()
	SelLog.log('I',"Manual mode")
elseif MAJORDOME_PAYLOAD == 'AUTO' then	-- Disable control timers
	t15s:Enable()
	t25s:Enable()
	SelLog.log('I',"Automatic mode")
elseif MAJORDOME_PAYLOAD == "WAITING" or MAJORDOME_PAYLOAD == "STOP" or
		MAJORDOME_PAYLOAD == "CHECKING" or MAJORDOME_PAYLOAD == "START" or
		MAJORDOME_PAYLOAD == "DONE" then
	tracker:setStatus( MAJORDOME_PAYLOAD )
end
