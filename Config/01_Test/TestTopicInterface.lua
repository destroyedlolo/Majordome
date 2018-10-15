-- This is an example of Lua topic's interface
--
-- Indicate MQTT topic(s) to listen to
-->> when=15s
--
-- Disable this script
--->> disabled

io.write('NonExistantTopic :')
local topic = MajordomeMQTTTopic.find("NonExistantTopic")
if not topic then
	print('Ok')
else
	print('Fail found in', topic:getContainer() )
end

io.write('NoStations :')
topic = MajordomeMQTTTopic.find("NoStations", true)
if not topic then
	print('Fail', topic)
else
	print('Ok found in', topic:getContainer() )

--[[
	if topic:isEnabled() then
		print "Now, it's disabled"
		topic:Disable()
	else
		print "Now, it's enabled"
		topic:Enable()

		print "Launching its tasks"
		topic:Launch()
	end
--]]
end

io.write('NonExistantTopic with failure :')
topic = MajordomeMQTTTopic.find("NonExistantTopic", true)

print "Never reached"
