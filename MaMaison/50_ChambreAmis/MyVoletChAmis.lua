-- Fermeture "My" du volet de la chambre d'amis
--
-- Suivit de la température
-->> whenDone=SurveillanceChAmis
--
-->> require_topic=ModeChAmis

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChAmis", true)

--
-- C'est parti
--

if ModeChAmis:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet de la chambre d'amis ignoré")
else
	SelLog.Log('A', "'My' du volet de la chambre d'amis")
	CmdVolet:Publish("My")
end
