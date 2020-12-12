-- Cette fonction lance la surveillance de température en fonction du mode de la chambre d'Océane
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceVacances
-->> when=OuvertureVoletChOceane

--
-- Récupération des objets cibles
--

local mode = SelShared.Get("ModeChOceane") or "Manuel"
local trackersurveillance = MajordomeTracker.find("SurveillanceChOceane", true)
local timerouverture = MajordomeTimer.find("OuvertureVoletChOceane", true)

-- Quelle est la consigne active
local consigne
if mode == "Vacances" then
	consigne = MajordomeTimer.find("DebutSurveillanceVacances", true)
else
	consigne = MajordomeTimer.find("DebutSurveillance", true)
end

--
-- C'est parti
--

if MAJORDOME_TIMER == consigne:getName() then	-- Lancé par la consigne
	if consigne:getAt() < timerouverture:getAt() then
		SelLog.log('D', "Le début de la surveillance de la chambre d'Océane est reporté (".. MAJORDOME_TIMER .. " < consigne )")
		return
	end

elseif MAJORDOME_TIMER == "OuvertureVoletChOceane" then -- lancé par l'heure de levé
	if consigne:getAt() > timerouverture:getAt() then	-- ignoré car ce n'est pas encore l'heure de la consigne
		SelLog.log('D', "Le début de la surveillance de la chambre d'Océane est reporté à consigne")
		return
	end

	SelLog.log('D', "Début de la surveillance de la chambre d'Océane est retardée de 30 secondes")
	Selene.Sleep(30);	-- on laisse les volets s'ouvrir

end

SelLog.log('D', "Début de la surveillance de la chambre d'Océane dû à ".. MAJORDOME_TIMER)
trackersurveillance:setStatus("START")

