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

--
-- C'est parti
--

-- quelle est la consigne de début de surveillance est a utilisée ?
local tmr_srv
if mode == "Vacances" then
	tmr_srv = MajordomeTimer.find("DebutSurveillanceVacances", true)
	MAJORDOME_TIMER = "DebutSurveillanceVacances"	-- force la raison du lancement
else
	tmr_srv = MajordomeTimer.find("DebutSurveillance", true)
	MAJORDOME_TIMER = "DebutSurveillance"
end
SelLog.log('D', "Consigne de début de surveillance ChParent : " .. tmr_srv:getAt());

-- vérifie que la consigne est < à l'heure de levé
if tmr_srv:getAt() < timerouverture:getAt() then
	SelLog.log('D', "Le début de la surveillance de la chambre des parents ignoré (".. MAJORDOME_TIMER .. " < consigne )")
	return
end

-------
-- Ajouter le mode DELSTART ou le démarrage démarre après 1 minute (en cas de démarrage par la
-- consigne de lever
------

	-- En fonction du mode, vérifie que c'est le bon timer qui tente de
	-- réveiller ce tracker
if (MAJORDOME_TIMER == "DebutSurveillance" and mode ~= "Vacances") or
   (MAJORDOME_TIMER == "DebutSurveillanceVacances" and mode == "Vacances")
then
	SelLog.log('D', "Début de la surveillance de la chambre des parents dû à ".. MAJORDOME_TIMER)
	trackersurveillance:setStatus("START")
else
	SelLog.log('D', "Le début de la surveillance de la chambre des parents dû à ".. MAJORDOME_TIMER .. " est ignoré")
end
