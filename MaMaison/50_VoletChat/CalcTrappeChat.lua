-- determine l'heure de fermeture des la trappe du chat
-->> listen=ModeDemain
-->> listen=Saison

local CmdTrappeChat = MajordomeMQTTTopic.find("CmdTrappeChat", true)
local MyTrappeChat = MajordomeTimer.find("MyTrappeChat", true)

if SelShared.Get("Saison") == 'Hiver' then
	SelLog.log('I', "'My' de la trappe du chat à 21h")
	MyTrappeChat:setAt( 21.00 )
elseif SelShared.Get("ModeDemain") == 'Travail' then
	SelLog.log('I', "'My' de la trappe du chat à 1h")
	MyTrappeChat:setAt( 1.00 )
else
	SelLog.log('I', "'My' de la trappe du chat à 2h")
	MyTrappeChat:setAt( 2.00 )
end
