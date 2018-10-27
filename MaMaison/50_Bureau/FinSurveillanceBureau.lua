-->> when=FinSurveillance

local TemperatureBureau = MajordomeMQTTTopic.find("TemperatureBureau", true)
local VoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)

SelLog.log('I', "Fin de la surveillance de la température du bureau")

if not TemperatureBureau:isEnabled() then	-- Le volet a été baissé grâce à la surveillance
	VoletBureau:Publish("Up")
	SelLog.log('A', 'Ouverture du volet du bureau')
else
	TemperatureBureau:Disable()
end
