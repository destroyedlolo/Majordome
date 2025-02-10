-- Fermeture "My" du volet de la cuisine
-->> when=FermetureTemperatureEteCuisine
-->> whenDone=SurveillanceSalon
-->> require_topic=Mode

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet de la cuisine ignoré")
else
	SelLog.Log('A', "'My' du volet de la cuisine")

	local CmdVoletCuisine = MajordomeMQTTTopic.find("CmdVoletCuisine", true)
	CmdVoletCuisine:Publish("My")
end
