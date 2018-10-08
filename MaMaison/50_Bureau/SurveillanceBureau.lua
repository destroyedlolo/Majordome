-- Surveillance de la température du bureau
-->> listen=TemperatureBureau

-- Consigne
local TLIMIT=21.5

local TemperatureBureau = MajordomeMQTTTopic.find("TemperatureBureau")
assert(TemperatureBureau, "'TemperatureBureau' pas trouvé")

local VoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau")
assert(VoletBureau, "'CmdVoletBureau' pas trouvé")

if MAJORDOME_PAYLOAD >= TLIMIT then
	SelLog.log('I', "Température du bureau, "..TLIMIT.." > à la consigne, Fin de la surveillance")
	SelLog.log('A', 'Fermeture "My" du volet du bureau')
	VoletBureau:Publish("My")

	TemperatureBureau:Disable()
end
