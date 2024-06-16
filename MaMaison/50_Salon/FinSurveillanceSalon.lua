-- Fin de la surveillance : on ouvre les volets s'ils ont été fermés
-->> whenStopped=SurveillanceSalon
-->> require_topic=Mode

--
-- Récupération des objets cibles
--

local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)

--
-- C'est parti
--

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet du salon ignoré")
else

		-- Note : on ne cherche pas la surveillance Fenêtre car elle est
		-- ici redondante avec la surveillance générale
	if MAJORDOME_TRACKER_STATUS == "DONE" then
		SelLog.Log('A', "Ouverture du volet du salon")
		CmdVoletSalonBalcon:Publish("Up")
		CmdVoletSalonCheminee:Publish("Up")
		CmdVoletSalonFenetre:Publish("Up")
	else
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température du salon")
	end
end
