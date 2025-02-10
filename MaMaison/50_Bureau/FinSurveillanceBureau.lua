-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceBureau

--
-- Récupération des objets cibles
--

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelSharedVar.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet du bureau ignoré")
else
	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.Log('A', "Ouverture du volet du bureau")
		CmdVoletBureau:Publish("Up")
	else
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température du bureau")
	end
end
