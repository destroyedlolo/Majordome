-- Cette fonction lance la surveillance de température en fonction du mode de la chambre d'amis
--
-- Ces 2 timers peuvent déclencher la surveillance
-->> when=DebutSurveillance
-->> when=DebutSurveillanceChAmisVacances

--
-- Récupération des objets cibles
--

local mode = SelShared.Get("ModeChAmis") or "Manuel"
local trackersurveillance = MajordomeTracker.find("SurveillanceChAmis", true)

--
-- C'est parti
--

if mode == "Absent" then
	trackersurveillance:Disable()	-- il sera réactivé si nécessaire lors de la détermination de la saison
	SelLog.log('D', "Le début de la surveillance de la chambre d'amis dû à ".. MAJORDOME_TIMER .. " est ignoré")
	return
end


if (MAJORDOME_TIMER == "DebutSurveillance" and mode ~= "Vacances") or
   (MAJORDOME_TIMER == "DebutSurveillanceChAmisVacances" and mode == "Vacances")
then
	SelLog.log('D', "Début de la surveillance de la chambre d'amis dû à ".. MAJORDOME_TIMER)
	trackersurveillance:setStatus("START")
else
	SelLog.log('D', "Le début de la surveillance de la chambre d'amis dû à ".. MAJORDOME_TIMER .. " est ignoré")
end
