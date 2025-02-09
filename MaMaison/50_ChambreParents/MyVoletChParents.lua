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
-->> require_topic=ModeChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)

--
-- C'est parti
--

if ModeChParents:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet de la chambre des parents ignoré")
else
	SelLog.Log('A', "'My' du volet de la chambre des parents")
	CmdVolet:Publish("My")
end
