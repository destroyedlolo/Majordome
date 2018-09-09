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

io.write('RunFor20s :')
task = MajordomeTask.find("RunFor20s")
if not task then
	print('Fail', task)
else
	print('Ok found in', task:getContainer() )

	if task:isEnabled() then
		print "Now, it's disabled"
		task:disable()
	else
		print "Now, it's enabled"
		task:enable()
	end
end

