-- Détermine s'il faut lancer la surveillance de la température
-->> listen=Saison
-->> until=DebutSurveillanceBureau

--
-- Vérification que tous les paramètres sont bien présents
--

if not SelShared.Get("Saison") then
	return
end

-- print("******* Lancé par trig:", MAJORDOME_TRIGGER, "tmr:", MAJORDOME_TIMER )

--
-- Récupération des timers
--

local TemperatureBureau = MajordomeMQTTTopic.find("TemperatureBureau")
assert(TemperatureBureau, "'TemperatureBureau' pas trouvé")

local DebutSurveillanceBureau = MajordomeTimer.find("DebutSurveillanceBureau")
assert(DebutSurveillanceBureau, "'DebutSurveillanceBureau' pas trouvé")
local debut = DebutSurveillanceBureau:getAt()

local FinSurveillanceBureau = MajordomeTimer.find("FinSurveillanceBureau")
assert(FinSurveillanceBureau, "'FinSurveillanceBureau' pas trouvé")
local fin = FinSurveillanceBureau:getAt()

local maintenant = tonumber(os.date("%H%M"))

--
-- Calcul s'il faut tester la température
--

if SelShared.Get("Saison") == "Hiver" then
	TemperatureBureau:Disable()
	SelLog.log('I', "C'est l'hiver : Pas de surveillance de la température du bureau")
elseif maintenant >= debut and maintenant < fin then
	TemperatureBureau:Enable()
	SelLog.log('I', "Debut de surveillance de la température du bureau")
else
--[[
	TemperatureBureau:Disable()
--]]
	SelLog.log('I', "Ce n'est pas l'heure : Pas de surveillance de la température du bureau")
end
