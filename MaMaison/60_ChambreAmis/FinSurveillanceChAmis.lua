-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChAmis

--
-- Récupération des objets cibles
--

local CmdVoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("ModeChAmis") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : Ouverture du volet de la chambre d'amis ignoré")
else
	local tracker = MajordomeTracker.find("SurveillanceChAmis")
	if not tracker then
		print("Can't find 'SurveillanceChAmis'")
		return
	end

	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.log('A', "Ouverture du volet de la chambre d'amis")
		CmdVoletChAmis:Publish("Up")
	else
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température de la chambre d'amis")
	end
end
