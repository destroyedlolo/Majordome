-- Consignes

--
-- Topics
--

HLEVE='Majordome/HLever'
HCOUCHE='Majordome/HCoucher'

--
-- Valeurs par défaut
--

SelShared.set( HLEVE, 8.00 )	-- Ne doit pas être < a 1:00
SelShared.set( HCOUCHE, 21.10 )	-- pour les enfants

--
-- Actions
--

function chgConsignes()
	SelLog.log("Heure levé :" .. SelShared.get( HLEVE ) )
	SelLog.log("Heure couché :" .. SelShared.get( HCOUCHE ) )

	SubTasks( Tasks['Consignes'] )
end

Tasks['Consignes']={}
table.insert( Topics, { topic = HLEVE, func=TopicDate2Number, trigger=chgConsignes, trigger_once=true } )
table.insert( Topics, { topic = HCOUCHE, func=TopicDate2Number, trigger=chgConsignes, trigger_once=true } )
