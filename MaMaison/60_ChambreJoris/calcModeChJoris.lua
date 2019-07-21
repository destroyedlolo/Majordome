-- Calcul du mode réel pour la chambre de Joris
-->> listen=Mode
-->> listen=ModeForceEnfants
-->> listen=ModeForceJoris

if not SelShared.Get("Mode") then
	return
end

local ModeChJoris = MajordomeMQTTTopic.find("ModeChJoris", true)
local res = SelShared.Get("ModeForceJoris") or "Auto"

if res == "Auto" then
	res = SelShared.Get("ModeForceEnfants") or "Auto"
	if res == "Auto" then
		res = SelShared.Get("Mode")
	end
end

ModeChJoris:Publish(res)
SelLog.log('M', "Le mode de la chambre de Joris est : " .. res)
