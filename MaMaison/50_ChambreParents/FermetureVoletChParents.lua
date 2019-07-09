-- Fermeture du volet de la chambre des parents
-->> when=ConsigneCoucher

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)
local mode = SelShared.Get("ModeChParents") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre des parents ignoré")
else
	SelLog.log('A', "Fermeture du volet de la chambre des parents")
	CmdVolet:Publish("Down")
end
