-- Recalcul de la surveillance de la température
-->> waitfor=recalculeSurveillance
-->> once

local TemperatureBureau = MajordomeMQTTTopic.find("TemperatureBureau", true)
local DebutSurveillance= MajordomeTimer.find("DebutSurveillance", true)
local FinSurveillance= MajordomeTimer.find("FinSurveillance", true)
local debut = DebutSurveillance:getAt()
local fin = FinSurveillance:getAt()

local maintenant = tonumber(os.date("%H.%M"))

if SelShared.Get("Saison") == "Hiver" then
	if TemperatureBureau:isEnabled() then
		TemperatureBureau:Disable()
	end
	SelLog.log('I', "C'est l'hiver : Pas de surveillance de la température du bureau")
elseif maintenant >= debut and maintenant < fin then
	if not TemperatureBureau:isEnabled() then
		TemperatureBureau:Enable()
		SelLog.log('I', "Debut de surveillance de la température du bureau")
	end
else
	if TemperatureBureau:isEnabled() then
		TemperatureBureau:Disable()
		SelLog.log('I', "Ce n'est pas l'heure : Pas de surveillance de la température du bureau")
	end
end
