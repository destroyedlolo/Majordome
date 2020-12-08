-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)
local mode = SelShared.Get("ModeChOceane") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : Ouverture du volet de la chambre d'Océane ignorée")
else
	local tracker = MajordomeTracker.find("SurveillanceChOceane")
	if not tracker then
		print("Can't find 'SurveillanceChOceane'")
		return
	end

	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.log('A', "Ouverture du volet de la chambre d'Océane")
		CmdVolet:Publish("Up")
	else
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température de la chambre d'Océane")
	end
end
