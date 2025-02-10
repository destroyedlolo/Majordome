-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceChJoris
-->> require_topic=ModeChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)

--
-- C'est parti
--

if ModeChJoris:getVal() == "Manuel" or mode == "Absent" then
	SelLog.Log('I', "Mode \"".. ModeChJoris:getVal() .."\" : Ouverture du volet de la chambre de Joris ignorée")
else
	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.Log('A', "Ouverture du volet de la chambre de Joris")
		CmdVolet:Publish("Up")
	else
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température de la chambre de Joris")
	end
end
