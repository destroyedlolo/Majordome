-- Maj du mode actif en fonction du mode calculé
-->> listen=ModeAujourdhui
-->> listen=ModeForce

local Mode = MajordomeMQTTTopic.find("Mode", true)

if SelShared.Get("ModeForce") == "Auto" then
	SelLog.log('M', "Mode calculé : " .. SelShared.Get("ModeAujourdhui"))
	Mode:Publish(SelShared.Get("ModeAujourdhui"))
end