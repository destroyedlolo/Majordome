-- Fermeture du volet de la chambre de Joris
-->> when=ConsigneCoucher

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)
local mode = SelShared.Get("ModeChJoris") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre de Joris ignoré")
else
	SelLog.log('A', "'Fermeture' du volet de la chambre de Joris")
	CmdVolet:Publish("Down")
end
