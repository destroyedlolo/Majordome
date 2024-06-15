-- Maj du mode actif en fonction du mode calculé
-->> listen=ModeForce
-->> listen=ModeAujourdhui
--
-->> need_topic=Mode
-->> need_topic=ModeForce
-->> need_topic=ModeAujourdhui


if ModeForce:getVal() == "Auto" then
	SelLog.Log('M', "Mode calculé : " .. ModeAujourdhui:getVal())
	Mode:Publish(ModeAujourdhui:getVal())
end
