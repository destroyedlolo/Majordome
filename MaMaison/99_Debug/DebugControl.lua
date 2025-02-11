-- This task will control the entire test
--
-- Indicate MQTT topic(s) to listen to
-->> listen=MajordomeDebug
--
-->> need_task=PublicationConsignesParDefaut
--
-- Disable this script
--->> disabled

local args = {}
for m in MAJORDOME_PAYLOAD:gmatch("%S+") do
	table.insert(args, m);
end

if args[1]:upper() == "TRACKER" then
	if #args < 3 then
		SelLog.Log('E',"TRACKER command needs 2 arguments")
		return
	end

	local tracker = MajordomeTracker.find(args[2])
	if not tracker then
		SelLog.Log('E', "Can't find '".. args[2].."'")
		return
	end
	if args[3]:upper() == 'STATUS' then
		SelLog.Log('D', "Tracker is coming from : ".. tracker:getContainer())
		SelLog.Log('D', "Tracker is ".. (tracker:isEnabled() and 'Enabled' or 'Disabled') )
		SelLog.Log('D', "Tracker's status is ".. tracker:getStatus())
	else
		tracker:setStatus( args[3]:upper() )
	end

elseif args[1]:upper() == "TIMER" then
	local timer = MajordomeTimer.find(args[2])
	if not timer then
		SelLog.Log('E', "Can't find '".. args[2].."'")
		return
	end

	local h,m = timer:getAtHM()
	SelLog.Log('D', "Timer is coming from : ".. timer:getContainer())
	SelLog.Log('D', "Timer is ".. (timer:isEnabled() and 'Enabled' or 'Disabled') )
	SelLog.Log('D', "Time at ".. ("%02d:%02d"):format(h,m))

elseif args[1]:upper() == "TOPIC" then
	local topic = MajordomeMQTTTopic.find(args[2])
	if not topic then
		SelLog.Log('E', "Can't find '".. args[2].."'")
		return
	end

	SelLog.Log('D', "Topic is coming from : ".. topic:getContainer())
	SelLog.Log('D', "Topic is ".. (topic:isEnabled() and 'Enabled' or 'Disabled') )

elseif args[1]:upper() == "LAUNCH" then
	local task = MajordomeTask.find(args[2])
	if not task then
		SelLog.Log('E', "Can't find '".. args[2].."'")
		return
	end
	task:Launch()

elseif args[1]:upper() == "SAISON" then
	local DetermineSaison = MajordomeTask.find("DetermineSaison")
	DetermineSaison:Launch()
elseif args[1]:upper() == "CONSIGNES" then
	local PublicationConsignesParDefault = MajordomeTask.find("PublicationConsignesParDefault")
	PublicationConsignesParDefaut:Launch()
else
	SelLog.Log('E',"Unknown command '"..args[1].."'")
end
