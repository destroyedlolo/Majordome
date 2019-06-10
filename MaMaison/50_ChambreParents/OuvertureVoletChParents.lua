-- Ouverture du volet de la chambre des parents
-->> when=OuvertureVoletChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)
local mode = SelShared.Get("ModeChParents") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre des parents ignoré")
else	
	SelLog.log('A', "Ouverture du volet de la chambre des parents")
	CmdVolet:Publish("Up")
end
