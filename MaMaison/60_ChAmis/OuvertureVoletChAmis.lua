-- Ouverture du volet de la chambre d'amis
-->> when=OuvertureVoletChAmis

local CmdVoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("Mode") or "Manuel"

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : 'Ouverture' du volet de la chambre d'amis ignoré")
else
	SelLog.log('A', "'Ouverture' du volet de la chambre d'amis")
	CmdVoletChAmis:Publish("Up")
end
