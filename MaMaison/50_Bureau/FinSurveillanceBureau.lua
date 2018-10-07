-->> when=FinSurveillance

local TemperatureBureau = MajordomeMQTTTopic.find("TemperatureBureau")
assert(TemperatureBureau, "'TemperatureBureau' pas trouvé")

local VoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau")
assert(VoletBureau, "'CmdVoletBureau' pas trouvé")

SelLog.log('I', "Fin de la surveillance de la température du bureau")

if not TemperatureBureau:isEnabled() then	-- Le volet a été baissé grâce à la surveillance
	VoletBureau:Publish("Up")
else
	TemperatureBureau:Disable()
end
