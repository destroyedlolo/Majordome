-- Cette fonction lance la surveillance de température en fonction du mode de la chambre des parents
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceVacances
-->> when=OuvertureVoletChParents

--
-- Récupération des objets cibles
--

local mode = SelShared.Get("ModeChParents") or "Manuel"
local trackersurveillance = MajordomeTracker.find("SurveillanceChParents", true)
local timerouverture = MajordomeTimer.find("OuvertureVoletChParents", true)

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
		SelLog.log('D', "Le début de la surveillance de la chambre des parents est reporté (".. MAJORDOME_TIMER .. " < consigne )")
		return
	end

elseif MAJORDOME_TIMER == "OuvertureVoletChParents" then -- lancé par l'heure de levé
	if consigne:getAt() > timerouverture:getAt() then	-- ignoré car ce n'est pas encore l'heure de la consigne
		SelLog.log('D', "Le début de la surveillance de la chambre des parents est reporté à consigne")
		return
	end

	SelLog.log('D', "Début de la surveillance de la chambre des parents est retardée de 30 secondes")
	Selene.Sleep(30);	-- on laisse les volets s'ouvrir

else
	return
end

SelLog.log('D', "Début de la surveillance de la chambre des parents dû à ".. MAJORDOME_TIMER)
trackersurveillance:setStatus("START")
