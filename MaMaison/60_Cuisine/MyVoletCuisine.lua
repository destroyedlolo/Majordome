-- Fermeture "My" du volet de la cuisine
-->> when=FermetureTemperatureEteCuisine
-->> whenDone=SurveillanceSalon

local mode = SelShared.Get("Mode") or "Manuel"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet de la cuisine ignoré")
else
	SelLog.log('A', "'My' du volet de la cuisine")

	local CmdVoletCuisine = MajordomeMQTTTopic.find("CmdVoletCuisine", true)
	CmdVoletCuisine:Publish("My")
end
