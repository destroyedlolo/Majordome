-- Ouverture du volet de la chambre d'Océane
-->> when=OuvertureVoletChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)
local mode = SelShared.Get("ModeChOceane") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre d'Océane ignorée")
else	
	SelLog.log('A', "Ouverture du volet de la chambre d'Océane")
	CmdVolet:Publish("Up")
end
