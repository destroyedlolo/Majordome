-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChOceane
-->> require_topic=ModeChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)

--
-- C'est parti
--

if ModeChOceane:getVal() == "Manuel" or ModeChOceane:getVal() == "Absent" then
	SelLog.Log('I', "Mode \"".. ModeChOceane:getVal() .."\" : Ouverture du volet de la chambre d'Océane ignorée")
else
	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.Log('A', "Ouverture du volet de la chambre d'Océane")
		CmdVolet:Publish("Up")
	else
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température de la chambre d'Océane")
	end
end
