-- This is an example of Lua task's interface
--
-- Indicate MQTT topic(s) to listen to
-->> listen=NoStations
--
-- And we have to create its object as well
-->> need_topic=NoStations
--
-- as well as task one
-->> need_task=ClientTask
--
-- Disable this script
--->> disabled

io.write('Trying to find NonExistantFunc : ')
local task = MajordomeTask.find("NonExistantFunc")
if not task then
	print('Ok')
else
	print('Fail, found in', task:getContainer() )
end

io.write('ClientTask is now ')

if ClientTask:isEnabled() then
	print "disabled"
	ClientTask:Disable()
else
	print "enabled"
	ClientTask:Enable()
end

