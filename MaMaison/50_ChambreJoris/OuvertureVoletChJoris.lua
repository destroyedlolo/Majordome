-- Ouverture du volet de la chambre de Joris
-->> when=OuvertureVoletChJoris
-->> require_topic=ModeChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)

--
-- C'est parti
--

if ModeChJoris:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet de la chambre de Joris ignorée")
else	
	SelLog.Log('A', "Ouverture du volet de la chambre de Joris")
	CmdVolet:Publish("Up")
end
