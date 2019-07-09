-- Fermeture du volet de la chambre d'Océane
-->> when=ConsigneCoucher

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)
local mode = SelShared.Get("ModeChOceane") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre d'Océane ignoré")
else
	SelLog.log('A', "Fermeture du volet de la chambre d'Océane")
	CmdVolet:Publish("Down")
end
