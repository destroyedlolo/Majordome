-- reactive le timer de la trappe du chat si le mode forcé est changé.
-->> listen=ModeForce

local TmrTrappeChat = MajordomeTimer.find("MyTrappeChat", true)

if SelShared.Get("ModeForce") ~= "Absent" then
	if not TmrTrappeChat:isEnabled() then
		TmrTrappeChat:Enable()
print("****************")
		SelLog.log('I', "Activation du timer de la trappe du chat")
	end
end
