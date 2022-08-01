-- Fermeture "My" du volet du salon
-->> whenDone=SurveillanceSalon

--
-- Récupération des objets cibles
--

local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
-- local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)
local mode = SelShared.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet du salon ignoré")
else
	SelLog.log('A', "Fermeture 'My' du volet du salon")
	CmdVoletSalonBalcon:Publish("My")
	CmdVoletSalonCheminee:Publish("My")
--	CmdVoletSalonFenetre:Publish("My")
end
