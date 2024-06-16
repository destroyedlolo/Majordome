-- Fermeture "My" du volet du salon
-->> whenDone=SurveillanceSalon
-->> require_topic=Mode

--
-- Récupération des objets cibles
--

local CmdVoletSalonBalcon = MajordomeMQTTTopic.find("CmdVoletSalonBalcon", true)
local CmdVoletSalonCheminee = MajordomeMQTTTopic.find("CmdVoletSalonCheminee", true)
-- local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)

--
-- C'est parti
--

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet du salon ignoré")
else
	SelLog.Log('A', "Fermeture 'My' du volet du salon")
	CmdVoletSalonBalcon:Publish("My")
	CmdVoletSalonCheminee:Publish("My")
--	CmdVoletSalonFenetre:Publish("My")
end
