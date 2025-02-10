-- Cette fonction lance la surveillance de température en fonction du mode de la chambre de Joris
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceVacances
-->> when=OuvertureVoletChJoris
--
-->> require_topic=ModeChJoris
-->> need_tracker=SurveillanceChJoris
-->> need_timer=OuvertureVoletChJoris

-- Quelle est la consigne active
local consigne
if ModeChJoris:getVal() == "Vacances" then
	consigne = MajordomeTimer.find("DebutSurveillanceVacances", true)
else
	consigne = MajordomeTimer.find("DebutSurveillance", true)
end

--
-- C'est parti
--

if MAJORDOME_TIMER == consigne:getName() then	-- Lancé par la consigne
	if consigne:getAt() < OuvertureVoletChJoris:getAt() then
		SelLog.Log('D', "Le début de la surveillance de la chambre de Joris est reporté (".. MAJORDOME_TIMER .. " < consigne )")
		return
	end

elseif MAJORDOME_TIMER == "OuvertureVoletChJoris" then -- lancé par l'heure de levé
	if consigne:getAt() > OuvertureVoletChJoris:getAt() then	-- ignoré car ce n'est pas encore l'heure de la consigne
		SelLog.Log('D', "Le début de la surveillance de la chambre de Joris est reporté à consigne")
		return
	end

	SelLog.Log('D', "Début de la surveillance de la chambre de Joris est retardée de 30 secondes")
	Selene.Sleep(30);	-- on laisse les volets s'ouvrir

else 
	return

end

SelLog.Log('D', "Début de la surveillance de la chambre de Joris dû à ".. MAJORDOME_TIMER)
SurveillanceChJoris:setStatus("START")

