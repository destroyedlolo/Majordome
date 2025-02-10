-- Ouverture du volet de la chambre d'amis
-->> when=OuvertureVoletChAmis
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
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre d'amis ignorée")
else	
	SelLog.Log('A', "Ouverture du volet de la chambre d'amis")
	CmdVolet:Publish("Up")
end
