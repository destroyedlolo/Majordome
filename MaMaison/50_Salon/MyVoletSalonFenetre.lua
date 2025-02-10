-- Fermeture "My" du volet du salon/Fenetre
-->> whenDone=SurveillanceSalonFenetre
-->> require_topic=Mode

--
-- Récupération des objets cibles
--

local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)

--
-- C'est parti
--

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet du salon/Fenetre ignoré")
else
	SelLog.Log('A', "Fermeture 'My' du volet du salon/Fenetre")
	CmdVoletSalonFenetre:Publish("My")
end
