-- Fermeture des volets en fin de journée
-->> waitfor=CoucherSoleilEffectif
--->> whenDone=CoucherSoleil
-->> require_topic=Mode

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Fermeture\" : Ouverture du volet de la cuisine ignoré")
else
	SelLog.Log('A', "Fermeture du volet de la cuisine")

	local CmdVoletCuisine = MajordomeMQTTTopic.find("CmdVoletCuisine", true)
	CmdVoletCuisine:Publish("Down")
end
