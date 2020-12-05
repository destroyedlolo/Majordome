-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)
local mode = SelShared.Get("ModeChJoris") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : Ouverture du volet de la chambre de Joris ignorée")
else
	local tracker = MajordomeTracker.find("SurveillanceChJoris")
	if not tracker then
		print("Can't find 'SurveillanceChJoris'")
		return
	end

	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.log('A', "Ouverture du volet de la chambre de Joris")
		CmdVolet:Publish("Up")
	else
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température de la chambre de Joris")
	end
end
