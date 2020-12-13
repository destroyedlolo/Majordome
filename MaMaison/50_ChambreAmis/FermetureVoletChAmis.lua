-- Fermeture du volet de la chambre d'amis
-->> when=FermetureVoletChAmis

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("ModeChAmis") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre d'amis ignoré")
else
	SelLog.log('A', "Fermeture du volet de la chambre d'amis")
	CmdVolet:Publish("Down")
end
