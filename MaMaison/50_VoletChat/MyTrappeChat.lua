-- Commande la mise en position "my" du volet de la trappe du chat
-->> when=TimerMyTrappeChat
-->> require_topic=Mode
-->> require_topic=ModeForce

local CmdTrappeChat = MajordomeMQTTTopic.find("CmdTrappeChat", true)

if Mode:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' de la trappe du chat ignoré")
else
	SelLog.Log('A', "'My' de la trappe du chat")
	CmdTrappeChat:Publish("My")
end

if ModeForce:getVal() == "Absent" then
	TimerMyTrappeChat:Disable()
	SelLog.Log('I', "Desactivation du timer de la trappe du chat")	-- Pour que le 'my' ne soit plus envoyé
end
