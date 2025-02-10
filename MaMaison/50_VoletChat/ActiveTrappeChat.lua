-- reactive le timer de la trappe du chat si le mode forcé est changé.
-->> listen=ModeForce
-->> require_topic=ModeForce
-->> need_timer=TimerMyTrappeChat

if ModeForce:getVal() ~= "Absent" then
	if not TimerMyTrappeChat:isEnabled() then
		TimerMyTrappeChat:Enable()
		SelLog.Log('I', "Activation du timer de la trappe du chat")
	end
end
