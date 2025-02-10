-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChAmis
-->> require_topic=ModeChAmis


--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChAmis", true)

--
-- C'est parti
--

if ModeChAmis:getVal() == "Manuel" or ModeChAmis:getVal() == "Absent" then
	SelLog.Log('I', "Mode \"".. ModeChAmis:getVal() .."\" : Ouverture du volet de la chambre d'amis ignorée")
else
	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.Log('A', "Ouverture du volet de la chambre d'amis")
		CmdVolet:Publish("Up")
	else
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température de la chambre d'amis")
	end
end
