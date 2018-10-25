-- Gestion de la pompe de la piscine
-->> listen=TarifElectricite
-->> listen=ModePiscine

local commande = MajordomeMQTTTopic.find("CmdPompePiscine", true)
local ModePiscine = MajordomeMQTTTopic.find("ModePiscine", true)

--[[ debug
print( "TarifElectricite", SelShared.Get("TarifElectricite") )
print( "ModePiscine",  ModePiscine:getVal() )
--]]

if ModePiscine:getVal() == 'Forcé' then
	SelLog.log('A', "Pompe piscine forcée");
	commande:Publish( "1" )
	SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'Forcé', true )
elseif ModePiscine:getVal() == 'Arret' then
	SelLog.log('A', "Pompe piscine Arrêtée");
	commande:Publish( "0" )
	SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'Arret', true )
elseif ModePiscine:getVal() == 'Heure Creuse' then
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
