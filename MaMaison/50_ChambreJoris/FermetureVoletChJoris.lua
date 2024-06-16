-- Fermeture du volet de la chambre de Joris
-->> when=ConsigneCoucher
-->> require_topic=ModeChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)

--
-- C'est parti
--

if ModeChJoris:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Fermeture du volet de la chambre de Joris ignoré")
else
	SelLog.Log('A', "Fermeture du volet de la chambre de Joris")
	CmdVolet:Publish("Down")
end
