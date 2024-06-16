-- Cette fonction lance la surveillance de température en fonction du mode de la chambre des parents
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceVacances
-->> when=OuvertureVoletChParents
--
-->> require_topic=ModeChParents
-->> need_tracker=SurveillanceChParents
-->> need_timer=OuvertureVoletChParents

--
-- Récupération des objets cibles
--

-- Quelle est la consigne active
local consigne
if ModeChParents:getVal() == "Vacances" then
	consigne = MajordomeTimer.find("DebutSurveillanceVacances", true)
else
	consigne = MajordomeTimer.find("DebutSurveillance", true)
end

--
-- C'est parti
--

if MAJORDOME_TIMER == consigne:getName() then	-- Lancé par la consigne
	if consigne:getAt() < OuvertureVoletChParents:getAt() then
		SelLog.Log('D', "Le début de la surveillance de la chambre des parents est reporté (".. MAJORDOME_TIMER .. " < consigne )")
		return
	end

elseif MAJORDOME_TIMER == "OuvertureVoletChParents" then -- lancé par l'heure de levé
	if consigne:getAt() > OuvertureVoletChParents:getAt() then	-- ignoré car ce n'est pas encore l'heure de la consigne
		SelLog.Log('D', "Le début de la surveillance de la chambre des parents est reporté à consigne")
		return
	end

	SelLog.Log('D', "Début de la surveillance de la chambre des parents est retardée de 30 secondes")
	Selene.Sleep(30);	-- on laisse les volets s'ouvrir

else
	return
end

SelLog.Log('D', "Début de la surveillance de la chambre des parents dû à ".. MAJORDOME_TIMER)
SurveillanceChParents:setStatus("START")
