-- Temperatures

--
-- Topics
--

TExterieur='maison/Temperature/Dehors'
TSalon='maison/Temperature/Salon'
TChOceane='maison/Temperature/Chambre Oceane'
TBureau='maison/Temperature/Bureau'

--
-- Température extérieur
--

function chgTExt()
	SubTasks( Tasks['TExterieur'] )
end

Tasks['TExterieur']={}
table.insert( Topics, { topic = TExterieur, func=Topic2Number, trigger=chgTExt, trigger_once=true } )

--
-- Température Salon
--

function chgTSalon()
	SubTasks( Tasks['TSalon'] )
end

Tasks['TSalon']={}
table.insert( Topics, { topic = TSalon, func=Topic2Number, trigger=chgTSalon, trigger_once=true } )

--
-- Température chambre Océane
--

function chgTChO()
	SubTasks( Tasks['TChOceane'] )
end

Tasks['TChOceane']={}
table.insert( Topics, { topic = TChOceane, func=Topic2Number, trigger=chgTChO, trigger_once=true } )

--
-- Température Bureau
--

function chgTBureau()
	SubTasks( Tasks['TBureau'] )
end

Tasks['TBureau']={}
table.insert( Topics, { topic = TBureau, func=Topic2Number, trigger=chgTBureau, trigger_once=true } )
