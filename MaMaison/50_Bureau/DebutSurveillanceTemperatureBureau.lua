-->> when=DebutSurveillance

local TemperatureBureau = MajordomeMQTTTopic.find("TemperatureBureau", true)

if not TemperatureBureau:isEnabled() then
	TemperatureBureau:Enable()
	SelLog.log('I', "Début de la surveillance de la température du bureau")
end
