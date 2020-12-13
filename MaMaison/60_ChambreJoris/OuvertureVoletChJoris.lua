-- Ouverture du volet de la chambre de Joris
-->> when=OuvertureVoletChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)
local mode = SelShared.Get("ModeChJoris") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre de Joris ignorée")
else	
	SelLog.log('A', "Ouverture du volet de la chambre de Joris")
	CmdVolet:Publish("Up")
end
