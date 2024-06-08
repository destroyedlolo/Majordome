-- This is an example of Lua timer's interface
--
-- Indicate MQTT topic(s) to listen to
-->> listen=NoStations
--
-- Disable this script
--->> disabled

io.write('NonExistantTimer :')
local timer = MajordomeTimer.find("NonExistantTimer")
if not timer then
	print('Ok')
else
	print('Fail found in', timer:getContainer() )
end

io.write('15s :')
timer = MajordomeTimer.find("15s")
if not timer then
	print('Fail', timer)
else
	print('Ok found ' ..timer:getName().. ' in', timer:getContainer() )

	if timer:isEnabled() then
		print "Now, it's disabled"
		timer:Disable()
	else
		print "Now, it's enabled"
		timer:Enable()

		print "Launching its tasks"
		timer:Launch()
	end
end

