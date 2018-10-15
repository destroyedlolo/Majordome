-- Commande la mise en position "my" du volet de la trappe du chat
-->> when=MyTrappeChat

local CmdTrappeChat = MajordomeMQTTTopic.find("CmdTrappeChat", true)

SelLog.log('A', "'My' de la trappe du chat")
CmdTrappeChat:Publish("My")
