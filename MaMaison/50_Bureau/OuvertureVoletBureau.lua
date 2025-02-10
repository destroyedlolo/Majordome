-- Ouverture du volet du bureau
-->> when=OuvertureVoletBureau

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelSharedVar.Get("Mode") or "Manuel"

if mode == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet du bureau ignoré")
else
	SelLog.Log('A', "Ouverture du volet du bureau")
	CmdVoletBureau:Publish("Up")
end
