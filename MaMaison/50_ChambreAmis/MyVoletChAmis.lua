-- Fermeture "My" du volet de la chambre d'amis
--
-- Suivit de la température
-->> whenDone=SurveillanceChAmis

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("ModeChAmis") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet de la chambre d'amis ignoré")
else
	SelLog.log('A', "'My' du volet de la chambre d'amis")
	CmdVolet:Publish("My")
end
