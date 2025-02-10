-- Ouverture des volets de la cuisine le matin
-->> when=LeverSoleil
-->> when=FinSurveillance
-->> whenStopped=SurveillanceSalon
-->> require_topic=Mode
-->> require_topic=Saison

-- Vérfication des conditions

if MAJORDOME_TIMER == "FinSurveillance" and Saison:getVal() ~= "Ete" then
	SelLog.Log('I', "Ouverture du volet de la cuisine par FinSurveillance ignoré car ce n'est pas l'été")
	return
elseif MAJORDOME_TRACKER == "SurveillanceSalon" then
	if MAJORDOME_TRACKER_STATUS ~= "DONE" then
		SelLog.Log('I', "Il n'y a pas eu d'économie sur la température de la cuisine")
		return
	end
end


-- C'est parti

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : Ouverture du volet de la cuisine ignoré")
else
	SelLog.Log('A', "Ouverture du volet de la cuisine")

	local CmdVoletCuisine = MajordomeMQTTTopic.find("CmdVoletCuisine", true)
	CmdVoletCuisine:Publish("Up")
end
