-- Fermeture du volet de la chambre d'amis
-->> when=FermetureVoletChAmis

--
-- Récupération des objets cibles
--

local CmdVoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("ModeChAmis") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : 'Fermeture' du volet de la chambre d'amis ignoré")
else
	SelLog.log('A', "Fermeture du volet de la chambre d'amis")
	CmdVoletChAmis:Publish("Down")
end
