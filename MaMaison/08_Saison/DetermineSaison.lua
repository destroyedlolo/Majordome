-- Détermination de la saison en fonction des températures.
-->> when=22h

SelLog.log('M',"Détermination de la saison")

local Saison = MajordomeMQTTTopic.find("Saison", true)
local SaisonHier = MajordomeMQTTTopic.find("SaisonHier", true)


SelLog.log('M',"T°Ext = " .. (SelShared.Get('TExterieur') or '??') .. " TSalon = " .. (SelShared.Get('TSalon') or '??') )

if not SelShared.Get('TExterieur') or not SelShared.Get('TSalon') then
	SelLog.log('M', "On attend l'arrivée des températures")

	local DetermineSaison2 = MajordomeTask.find("DetermineSaison2Chance", true)
	DetermineSaison2:Enable() -- On attend l'arrivée de la température
else
	SeleMQTT.Publish( SaisonHier:getTopic(), Saison:getVal() )
	if SelShared.Get('TExterieur') < 5 then
		SeleMQTT.Publish( Saison:getTopic(), 'Hiver' );
	elseif SelShared.Get('TSalon') > 21.5 and SelShared.Get('TExterieur') > 15 then
		SeleMQTT.Publish( Saison:getTopic(), 'Ete' );
	else
		SeleMQTT.Publish( Saison:getTopic(), 'Intersaison' );
	end

	SelLog.log('M', "Ok")
end
