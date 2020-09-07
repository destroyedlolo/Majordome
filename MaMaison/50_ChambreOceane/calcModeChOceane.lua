-- Calcul du mode réel pour la chambre d'Oceane
-->> listen=Mode
-->> listen=ModeForceEnfants
-->> listen=ModeForceOceane

if not SelShared.Get("Mode") then -- Appel avant l'acquisition du mode
	return
end

local ModeChOceane = MajordomeMQTTTopic.find("ModeChOceane", true)
local res = SelShared.Get("ModeForceOceane") or "Auto"

if res == "Auto" then
	res = SelShared.Get("ModeForceEnfants") or "Auto"
	if res == "Auto" then
		res = SelShared.Get("Mode")
	end
end

ModeChOceane:Publish(res)
SelLog.log('M', "Le mode de la chambre d'Océane est : " .. res)
