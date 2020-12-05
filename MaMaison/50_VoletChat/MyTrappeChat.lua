-- Commande la mise en position "my" du volet de la trappe du chat
-->> when=MyTrappeChat

local CmdTrappeChat = MajordomeMQTTTopic.find("CmdTrappeChat", true)
local mode = SelShared.Get("Mode") or "Manuel"
local modeforce = SelShared.Get("ModeForce") or "Manuel"

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' de la trappe du chat ignoré")
else
	SelLog.log('A', "'My' de la trappe du chat")
	CmdTrappeChat:Publish("My")
end

if modeforce == "Absent" then
	local TmrTrappeChat = MajordomeTimer.find("MyTrappeChat", true)
	TmrTrappeChat:Disable()
	SelLog.log('I', "Desactivation du timer de la trappe du chat")	-- Pour que le 'my' ne soit plus envoyé
end
