-- This is an example of Lua task's interface
--
-- Indicate MQTT topic(s) to listen to
-->> listen=NoStations
--
-- Disable this script
--->> disabled

io.write('NonExistantFunc :')
local task = MajordomeTask.find("NonExistantFunc")
if not task then
	print('Ok')
else
	print('Fail found in', task:getContainer() )
end

io.write('DoNothing :')
task = MajordomeTask.find("DoNothing")
if not task then
	print('Fail', task)
else
	print('Ok found in', task:getContainer() )
end

