-- Fermeture "My" du volet du bureau
-->> whenDone=SurveillanceBureau

--
-- Récupération des objets cibles
--

local CmdVoletBureau = MajordomeMQTTTopic.find("CmdVoletBureau", true)
local mode = SelSharedVar.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet du bureau ignoré")
else
	SelLog.Log('A', "Fermeture 'My' du volet du bureau")
	CmdVoletBureau:Publish("My")
end
