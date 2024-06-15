-- Maj du mode de demain
-->> listen=ModeForce
-->> listen=ModeDemain
--
-->> need_topic=Mode
-->> need_topic=ModeForce
-->> need_topic=ModeDemain

if ModeForce:getVal() ~= "Auto" and ModeForce:getVal() ~= ModeDemain:getVal() then
	SelLog.Log('M', '"Mode Demain" forcé à : ' .. ModeDemain:getVal())
	ModeDemain:Publish(ModeForce:getVal())
end
