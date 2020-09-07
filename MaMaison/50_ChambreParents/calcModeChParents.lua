-- Calcul du mode réel pour la chambre de parents
-->> listen=Mode
-->> listen=ModeForceParents

if not SelShared.Get("Mode") then -- Appel avant que le mode soit déterminé
	return
end

local ModeChParents = MajordomeMQTTTopic.find("ModeChParents", true)
local res = SelShared.Get("ModeForceParents") or "Auto"

if res == "Auto" then
	res = SelShared.Get("Mode")
end

ModeChParents:Publish(res)
SelLog.log('M', "Le mode de la chambre des parents est : " .. res)
