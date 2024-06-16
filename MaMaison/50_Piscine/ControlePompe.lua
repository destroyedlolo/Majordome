-- Gestion de la pompe de la piscine
-->> listen=TarifElectricite
-->> listen=ModePiscine
-->> when=DebutCaniculeApresMidi
-->> when=FinCaniculeApresMidi
-->> need_topic=ModePiscine
-->> require_topic=TarifElectricite

local CmdPompePiscine = MajordomeMQTTTopic.find("CmdPompePiscine", true)

--[[ debug
print( "TarifElectricite", TarifElectricite:getVal() )
print( "ModePiscine",  ModePiscine:getVal() )
--]]

if ModePiscine:getVal() == 'Canicule'  then	-- activation supplémentaires par timers
	if MAJORDOME_TIMER == "DebutCaniculeApresMidi" then
		SelLog.Log('A', "Pompe piscine activée");
		CmdPompePiscine:Publish( "1" )
		MQTTBroker:Publish( ModePiscine:getTopic() .. '/status', 'HC1', true )
		return
	elseif MAJORDOME_TIMER == "FinCaniculeApresMidi" then
		SelLog.Log('A', "Pompe piscine stoppée");
		CmdPompePiscine:Publish( "0" )
		MQTTBroker:Publish( ModePiscine:getTopic() .. '/status', 'HC0', true )
		return
	end
end

if ModePiscine:getVal() == 'Forcé' then
	SelLog.Log('A', "Pompe piscine forcée");
	CmdPompePiscine:Publish( "1" )
	MQTTBroker:Publish( ModePiscine:getTopic() .. '/status', 'Forcé', true )
elseif ModePiscine:getVal() == 'Arret' then
	SelLog.Log('A', "Pompe piscine Arrêtée");
	CmdPompePiscine:Publish( "0" )
	MQTTBroker:Publish( ModePiscine:getTopic() .. '/status', 'Arret', true )
elseif ModePiscine:getVal() == 'Heures Creuses' or ModePiscine:getVal() == 'Canicule' then
		-- Les heures creusent déclenchent aussi la pompe en mode Canicule
	if TarifElectricite:getVal() == 'HC..' then
		SelLog.Log('A', "Pompe piscine activée");
		CmdPompePiscine:Publish( "1" )
		MQTTBroker:Publish( ModePiscine:getTopic() .. '/status', 'HC1', true )
	else
		SelLog.Log('A', "Pompe piscine stoppée");
		CmdPompePiscine:Publish( "0" )
		MQTTBroker:Publish( ModePiscine:getTopic() .. '/status', 'HC0', true )
	end
else
	SelLog.Log('E', "Mode piscine non identifié");
end
