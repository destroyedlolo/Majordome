-- Gestion de la pompe de la piscine
-->> listen=TarifElectricite
-->> listen=ModePiscine
--
-- Config
local Topic_pompe = "maison/IO/Pompe_Piscine"
-- fin config

local ModePiscine = MajordomeMQTTTopic.find("ModePiscine")
assert( ModePiscine, "'ModePiscine' pas trouvé" );

-- debug
print( "TarifElectricite", SelShared.Get("TarifElectricite") )
print( "ModePiscine",  ModePiscine:getVal() )
--

if ModePiscine:getVal() == 'Forcé' then
	SelLog.log('A', "Pompe piscine forcée");
	SeleMQTT.Publish( Topic_pompe, 1 )
	SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'Forcé', true )
elseif ModePiscine:getVal() == 'Arret' then
	SelLog.log('A', "Pompe piscine Arrêtée");
	SeleMQTT.Publish( Topic_pompe, 0 )
	SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'Arret', true )
elseif ModePiscine:getVal() == 'Heure Creuse' then
	if SelShared.Get("TarifElectricite") == 'HC..' then
		SelLog.log('A', "Pompe piscine activée");
		SeleMQTT.Publish( Topic_pompe, 1 )
		SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'HC1', true )
	else
		SelLog.log('A', "Pompe piscine stoppée");
		SeleMQTT.Publish( Topic_pompe, 0 )
		SeleMQTT.Publish( ModePiscine:getTopic() .. '/status', 'HC0', true )
	end
else
	SelLog.log('E', "Mode piscine non identifié");
end
