-- Ouverture des volets de la cuisine le matin
-->> when=LeverSoleil
-->> when=FinSurveillance
-->> whenStopped=SurveillanceSalon

local mode = SelShared.Get("Mode") or "Manuel"
local saison = SelShared.Get("Saison") or "Hiver"


-- Vérfication des conditions

if MAJORDOME_TIMER == "FinSurveillance" and saison ~= "Ete" then
	SelLog.log('I', "Ouverture du volet de la cuisine par FinSurveillance ignoré car ce n'est pas l'été")
	return
elseif MAJORDOME_TRACKER == "SurveillanceSalon" then
	if MAJORDOME_TRACKER_STATUS ~= "DONE" then
		SelLog.log('I', "Il n'y a pas eu d'économie sur la température de la cuisine")
		return
	end
end


-- C'est parti

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : Ouverture du volet de la cuisine ignoré")
else
	SelLog.log('A', "Ouverture du volet de la cuisine")

	local CmdVoletCuisine = MajordomeMQTTTopic.find("CmdVoletCuisine", true)
	CmdVoletCuisine:Publish("Up")
end
