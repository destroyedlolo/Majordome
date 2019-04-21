-- Fermeture "My" du volet du bureau
-->> whenDone=SurveillanceBureau

--
-- Récupération des objets cibles
--

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelShared.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet du bureau ignoré")
else
	SelLog.log('A', "Fermeture 'My' du volet du bureau")
	CmdVoletBureau:Publish("My")
end
