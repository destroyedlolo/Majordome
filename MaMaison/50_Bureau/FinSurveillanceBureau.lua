-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceBureau

--
-- Récupération des objets cibles
--

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelShared.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet du bureau ignoré")
else
	local tracker = MajordomeTracker.find("SurveillanceBureau")
	if not tracker then
		print("Can't find 'SurveillanceBureau'")
		return
	end

	if tracker:getStatus() == "DONE" then
		SelLog.log('A', "Ouverture du volet du bureau")
		CmdVoletBureau:Publish("Up")
	else
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température du bureau")
	end
end
