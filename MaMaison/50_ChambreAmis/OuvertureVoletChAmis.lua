-- Ouverture du volet de la chambre d'amis
-->> when=OuvertureVoletChAmis

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("ModeChAmis") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre d'amis ignorée")
else	
	SelLog.log('A', "Ouverture du volet de la chambre d'amis")
	CmdVolet:Publish("Up")
end
