-- Fermeture "My" du volet du salon/Fenetre
-->> whenDone=SurveillanceSalonFenetre

--
-- Récupération des objets cibles
--

local CmdVoletSalonFenetre = MajordomeMQTTTopic.find("CmdVoletSalonFenetre", true)
local mode = SelShared.Get("Mode") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet du salon/Fenetre ignoré")
else
	SelLog.log('A', "Fermeture 'My' du volet du salon/Fenetre")
	CmdVoletSalonFenetre:Publish("My")
end
