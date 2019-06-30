-- Fermeture des volets en fin de journée
-->> when=CoucherSoleil

local mode = SelShared.Get("Mode") or "Manuel"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Fermeture\" : 'Ouverture' du volet de la cuisine ignoré")
else
	SelLog.log('A', "'Fermeture' du volet de la cuisine")

	local CmdVoletCuisine = MajordomeMQTTTopic.find("CmdVoletCuisine", true)
	CmdVoletCuisine:Publish("Down")
end
