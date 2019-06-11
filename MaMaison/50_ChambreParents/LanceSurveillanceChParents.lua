-- Cette fonction lance la surveillance de température en fonction du mode de la chambre des parents
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceVacances

--
-- Récupération des objets cibles
--

local mode = SelShared.Get("ModeChParents") or "Manuel"
local trackersurveillance = MajordomeTracker.find("SurveillanceChParents", true)

--
-- C'est parti
--

if (MAJORDOME_TIMER == "DebutSurveillance" and mode ~= "Vacances") or
   (MAJORDOME_TIMER == "DebutSurveillanceVacances" and mode == "Vacances")
then
	SelLog.log('D', "Début de la surveillance de la chambre des parents dû à ".. MAJORDOME_TIMER)
	trackersurveillance:setStatus("START")
else
	SelLog.log('D', "Le début de la surveillance de la chambre des parents dû à ".. MAJORDOME_TIMER .. " est ignoré")
end
