-- Production électrique

--
-- Topics
--

TProdElectrique='TeleInfo/Production/values/PAPP'

function chgTeleInfoProd()
	SubTasks( Tasks['TeleInfoProd'] )
end

function rcvTProd(t, v)
	v = tonumber(v)
	if v ~= 0 then	-- Si null, on laisse l'informatione expirer.
		Topic2NumberTTL( t,v, 60 )	-- Après 1 minutes, on estime avoir perdu la Teleinfo ou qu'il n'y a plus de production
	end
	return true
end

Tasks['TeleInfoProd']={}
table.insert( Topics, { topic=TProdElectrique, func=rcvTProd, trigger=chgTeleInfoProd, trigger_once=true } )
