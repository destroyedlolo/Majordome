-- Cette fonction lance la surveillance de température en fonction du mode de la chambre d'amis
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillanceChAmis
-->> when=DebutSurveillanceVacances
-->> when=OuvertureVoletChAmis
--
-->> need_tracker=SurveillanceChAmis
-->> need_timer=OuvertureVoletChAmis
-->> require_topic=ModeChAmis

-- Quelle est la consigne active
local consigne
if ModeChAmis:getVal() == "Vacances" then
	consigne = MajordomeTimer.find("DebutSurveillanceVacances", true)
else
	consigne = MajordomeTimer.find("DebutSurveillanceChAmis", true)
end

--
-- C'est parti
--

if MAJORDOME_TIMER == consigne:getName() then	-- Lancé par la consigne
	if consigne:getAt() < OuvertureVoletChAmis:getAt() then
		SelLog.Log('D', "Le début de la surveillance de la chambre d'amis est reporté (".. MAJORDOME_TIMER .. " < consigne )")
		return
	end

elseif MAJORDOME_TIMER == "OuvertureVoletChAmis" then -- lancé par l'heure de levé
	if consigne:getAt() > OuvertureVoletChAmis:getAt() then	-- ignoré car ce n'est pas encore l'heure de la consigne
		SelLog.Log('D', "Le début de la surveillance de la chambre d'amis est reporté à consigne")
		return
	end

	SelLog.Log('D', "Début de la surveillance de la chambre d'amis est retardée de 30 secondes")
	Selene.Sleep(30);	-- on laisse les volets s'ouvrir

else 
	return
end

SelLog.Log('D', "Début de la surveillance de la chambre d'amis dû à ".. MAJORDOME_TIMER)
SurveillanceChAmis:setStatus("START")

