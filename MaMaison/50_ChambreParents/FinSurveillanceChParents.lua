-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChParents
-->> require_topic=ModeChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)

--
-- C'est parti
--

if ModeChParents:getVal() == "Manuel" or ModeChParents:getVal() == "Absent" then
	SelLog.Log('I', "Mode \"".. ModeChParents:getVal() .."\" : Ouverture du volet de la chambre des parents ignorée")
else
	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.Log('A', "Ouverture du volet de la chambre des parents")
		CmdVolet:Publish("Up")
	else
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température de la chambre de parents")
	end
end
