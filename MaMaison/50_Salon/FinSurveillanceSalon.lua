-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceSalon

--
-- Récupération des objets cibles
--

local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)local mode = SelShared.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet du salon ignoré")
else
	local tracker = MajordomeTracker.find("SurveillanceSalon")
	if not tracker then
		print("Can't find 'SurveillanceSalon'")
		return
	end

	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.log('A', "Ouverture du volet du salon")
		CmdVoletSalonBalcon:Publish("Up")
		CmdVoletSalonCheminee:Publish("Up")
		CmdVoletSalonFenetre:Publish("Up")
	else
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température du salon")
	end
end
