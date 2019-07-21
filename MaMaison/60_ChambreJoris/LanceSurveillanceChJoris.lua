-- Cette fonction lance la surveillance de température en fonction du mode de la chambre de Joris
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceVacances

--
-- Récupération des objets cibles
--

local mode = SelShared.Get("ModeChJoris") or "Manuel"
local trackersurveillance = MajordomeTracker.find("SurveillanceChJoris", true)

--
-- C'est parti
--

if (MAJORDOME_TIMER == "DebutSurveillance" and mode ~= "Vacances") or
   (MAJORDOME_TIMER == "DebutSurveillanceVacances" and mode == "Vacances")
then
	SelLog.log('D', "Début de la surveillance de la chambre de Joris dû à ".. MAJORDOME_TIMER)
	trackersurveillance:setStatus("START")
else
	SelLog.log('D', "Le début de la surveillance de la chambre de Joris dû à ".. MAJORDOME_TIMER .. " est ignoré")
end
