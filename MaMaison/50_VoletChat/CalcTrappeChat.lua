-- determine l'heure de fermeture des la trappe du chat
-->> listen=ModeDemain
-->> listen=Saison

local CmdTrappeChat = MajordomeMQTTTopic.find("CmdTrappeChat")
assert(CmdTrappeChat, "'CmdTrappeChat' pas trouvé")

local MyTrappeChat = MajordomeTimer.find("MyTrappeChat")
assert(MyTrappeChat, "'MyTrappeChat' pas trouvé")

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
