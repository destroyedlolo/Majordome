-- This task will control the entire test
--
-- Indicate MQTT topic(s) to listen to
-->> listen=MajordomeDebug
--
-- Disable this script
--->> disabled

local args = {}
for m in MAJORDOME_PAYLOAD:gmatch("%S+") do
	table.insert(args, m);
end

if args[1]:upper() == "TRACKER" then
	local tracker = MajordomeTracker.find(args[2])
	if not tracker then
		print("Can't find '".. args[2].."'")
		return
	end
	if args[3]:upper() == 'STATUS' then
		SelLog.log('D', "Tracker is coming from : ".. tracker:getContainer())
		SelLog.log('D', "Tracker is ".. (tracker:isEnabled() and 'Enabled' or 'Disabled') )
		SelLog.log('D', "Tracker's status is ".. tracker:getStatus())
	else
		tracker:setStatus( args[3]:upper() )
	end
else
	SelLog.log('E',"Command inconnue '"..args[1].."'")
end
