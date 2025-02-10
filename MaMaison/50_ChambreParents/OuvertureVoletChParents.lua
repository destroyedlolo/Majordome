-- Ouverture du volet de la chambre des parents
-->> when=OuvertureVoletChParents
-->> require_topic=ModeChParents

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)

--
-- C'est parti
--

if ModeChParents:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre des parents ignorée")
else	
	SelLog.Log('A', "Ouverture du volet de la chambre des parents")
	CmdVolet:Publish("Up")
end
