-- Ouverture du volet de la chambre d'Océane
-->> when=OuvertureVoletChOceane
-->> require_topic=ModeChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)

--
-- C'est parti
--

if ModeChOceane:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre d'Océane ignorée")
else	
	SelLog.Log('A', "Ouverture du volet de la chambre d'Océane")
	CmdVolet:Publish("Up")
end
