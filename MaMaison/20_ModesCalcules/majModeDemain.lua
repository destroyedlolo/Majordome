-- Maj du mode de demain
-->> listen=ModeDemain
-->> listen=ModeForce

local ModeDemain = MajordomeMQTTTopic.find("ModeDemain", true)

if SelShared.Get("ModeForce") ~= "Auto" and SelShared.Get("ModeForce") ~= ModeDemain:getVal() then
	SelLog.log('M', '"Mode Demain" forcé à : ' .. SelShared.Get("ModeForce"))
	ModeDemain:Publish(SelShared.Get("ModeForce"))
end
