-- Ouverture du volet du bureau
-->> when=OuvertureVoletBureau

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelShared.Get("Mode") or "Manuel"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet du bureau ignoré")
else
	SelLog.log('A', "Ouverture du volet du bureau")
	CmdVoletBureau:Publish("Up")
end
