-- Fermeture "My" du volet de ma chambre d'amis
-->> whenDone=SurveillanceChAmis

--
-- Récupération des objets cibles
--

local CmdVoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local mode = SelShared.Get("ModeChAmis") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" or mode == "Absent" then
	SelLog.log('I', "Mode \"".. mode .."\" : 'My' du volet de la chambre d'amis ignoré")
else
	SelLog.log('A', "Fermeture 'My' de la chambre d'amis")
	CmdVoletChAmis:Publish("My")
end
