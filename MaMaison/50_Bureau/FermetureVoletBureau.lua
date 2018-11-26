-- Ouverture du volet du bureau
-->> when=FermetureVoletBureau

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelShared.Get("Mode") or "Manuel"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'Fermeture' du volet du bureau ignoré")
else
	SelLog.log('A', "'Fermeture' du volet du bureau")
	CmdVoletBureau:Publish("Down")
end
