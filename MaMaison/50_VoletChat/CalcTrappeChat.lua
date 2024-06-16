-- determine l'heure de fermeture des la trappe du chat
-->> listen=ModeDemain
-->> require_topic=ModeDemain
-->> listen=Saison
-->> require_topic=Saison
-->> need_timer=TimerMyTrappeChat
--
local CmdTrappeChat = MajordomeMQTTTopic.find("CmdTrappeChat", true)

if Saison:getVal() == 'Hiver' then
	SelLog.Log('I', "'My' de la trappe du chat à 21h")
	TimerMyTrappeChat:setAt( 21.00 )
elseif ModeDemain:getVal() == 'Travail' then
	SelLog.Log('I', "'My' de la trappe du chat à 1h")
	TimerMyTrappeChat:setAt( 1.00 )
else
	SelLog.Log('I', "'My' de la trappe du chat à 2h")
	TimerMyTrappeChat:setAt( 2.00 )
end
