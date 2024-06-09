-- This is an example of Lua timer's interface
--
-- Indicate MQTT topic(s) to listen to
-->> listen=NoStations
--
-- Create the timer object
-->> need_timer=5s
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

io.write('5s timer : ')

if _G['5s']:isEnabled() then
	print "disabled"
	_G['5s']:Disable()
else
	print "enabled"
	_G['5s']:Enable()

	print "Launching its tasks"
	_G['5s']:Launch()
end

