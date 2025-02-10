-- Fermeture du volet de la chambre d'amis
-->> when=FermetureVoletChAmis
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
	SelLog.Log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre d'amis ignoré")
else
	SelLog.Log('A', "Fermeture du volet de la chambre d'amis")
	CmdVolet:Publish("Down")
end
