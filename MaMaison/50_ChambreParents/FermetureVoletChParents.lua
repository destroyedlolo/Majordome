-- Fermeture du volet de la chambre des parents
-->> when=ConsigneCoucher
-->> require_topic=ModeChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)

--
-- C'est parti
--

if ModeChParents:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'Fermeture' du volet de la chambre des parents ignoré")
else
	SelLog.Log('A', "Fermeture du volet de la chambre des parents")
	CmdVolet:Publish("Down")
end
