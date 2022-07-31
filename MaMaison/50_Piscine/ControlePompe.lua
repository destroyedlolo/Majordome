-- Gestion de la pompe de la piscine
-->> listen=TarifElectricite
-->> listen=ModePiscine
-->> when=DebutCaniculeApresMidi
-->> when=FinCaniculeApresMidi

local commande = MajordomeMQTTTopic.find("CmdPompePiscine", true)
local ModePiscine = MajordomeMQTTTopic.find("ModePiscine", true)

--[[ debug
print( "TarifElectricite", SelShared.Get("TarifElectricite") )
print( "ModePiscine",  ModePiscine:getVal() )
--]]

if ModePiscine:getVal() == 'Canicule'  then	-- activation supplémentaires par timers
	if MAJORDOME_TIMER == "DebutCaniculeApresMidi" then
		SelLog.log('A', "Pompe piscine activée");
		commande:Publish( "1" )
		SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'HC1', true )
		return
	elseif MAJORDOME_TIMER == "FinCaniculeApresMidi" then
		SelLog.log('A', "Pompe piscine stoppée");
		commande:Publish( "0" )
		SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'HC0', true )
		return
	end
end

if ModePiscine:getVal() == 'Forcé' then
	SelLog.log('A', "Pompe piscine forcée");
	commande:Publish( "1" )
	SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'Forcé', true )
elseif ModePiscine:getVal() == 'Arret' then
	SelLog.log('A', "Pompe piscine Arrêtée");
	commande:Publish( "0" )
	SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'Arret', true )
elseif ModePiscine:getVal() == 'Heures Creuses' or ModePiscine:getVal() == 'Canicule'  then
		-- Les heures creusent déclenchent aussi la pompe en mode Canicule
	if SelShared.Get("TarifElectricite") == 'HC..' then
		SelLog.log('A', "Pompe piscine activée");
		commande:Publish( "1" )
		SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'HC1', true )
	else
		SelLog.log('A', "Pompe piscine stoppée");
		commande:Publish( "0" )
		SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'HC0', true )
	end
else
	SelLog.log('E', "Mode piscine non identifié");
end
