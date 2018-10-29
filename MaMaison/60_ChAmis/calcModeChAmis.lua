-- Calcul du mode réel pour la chambre d'amis
-->> listen=Mode
-->> listen=ModeForceChAmis

if not SelShared.Get("Mode")
or not SelShared.Get("ModeForceChAmis") then
	return
end

local ModeChAmis = MajordomeMQTTTopic.find("ModeChAmis", true)
local res = SelShared.Get("ModeForceChAmis")

if SelShared.Get("ModeForceChAmis") == "Auto" then
	res = SelShared.Get("Mode")
end

ModeChAmis:Publish(res)
SelLog.log('M', "Le mode de la chambre d'amis est : " .. res)
