-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)
local mode = SelShared.Get("ModeChParents") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : Ouverture du volet de la chambre des parents ignorée")
else
	local tracker = MajordomeTracker.find("SurveillanceChParents")
	if not tracker then
		print("Can't find 'SurveillanceChParents'")
		return
	end

	if tracker:getStatus() == "DONE" then
		SelLog.log('A', "Ouverture du volet de la chambre des parents")
		CmdVolet:Publish("Up")
	else
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température de la chambre de parents")
	end
end
