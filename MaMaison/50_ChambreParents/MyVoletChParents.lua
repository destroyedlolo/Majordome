-- Fermeture "My" du volet de la chambre des parents
--
-- Ouverture du matin
-->> when=OuvertureVoletChParentsMy
--
-- Fermeture du soir (pas nécessaire pour la chambre des parents)
--->> when=ConsigneCoucherMy
--
-- Suivit de la température
-->> whenDone=SurveillanceChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)
local mode = SelShared.Get("ModeChParents") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet de la chambre des parents ignoré")
else
	SelLog.log('A', "'My' du volet de la chambre des parents")
	CmdVolet:Publish("My")
end
