-- Fermeture du volet de la chambre d'Océane
-->> when=ConsigneCoucher
-->> require_topic=ModeChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)

--
-- C'est parti
--

if ModeChOceane:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre d'Océane ignoré")
else
	SelLog.Log('A', "Fermeture du volet de la chambre d'Océane")
	CmdVolet:Publish("Down")
end
