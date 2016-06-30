-- Seasons

--
-- Topics
--

SAISON='Majordome/Saison'
SAISONHIER='Majordome/Saison/Hier'

--
-- Valeurs par défaut
--

SelShared.set( SAISON, 'Intersaison' )
SelShared.set( SAISONHIER, 'Intersaison' )

--
-- Actions
--

function DetermineSaison()
	local saison

	if SelShared.get( TExterieur ) < 2 then
		saison = 'Hiver'
	elseif SelShared.get( TSalon ) > 23 then
		saison = 'Ete'
	else
		saison = 'Intersaison'
	end
	SelLog.log("Détermination de la saison")
	SelLog.log("T°Ext = " .. SelShared.get( TExterieur ) .. " TSalon = " .. SelShared.get( TSalon ) .. " => " .. saison)

	local psaison = SelShared.get( SAISON )
	SelShared.set( SAISON, nil )	-- Pour éviter que chgSaison soit appelé 2 fois
	Brk:Publish( SAISONHIER, psaison, true )
	Brk:Publish( SAISON, saison, true )
end

tmrAddEntry( tbl_timers, 5.0, DetermineSaison )


function chgSaison()
	SelLog.log("La Saison est maintenant : " .. SelShared.get(SAISON))

	if SelShared.get( SAISONHIER ) then
		SubTasks( Tasks['Saison'] )
	end
end

function chgSaisonHier()
	SelLog.log("La Saison d'hier est maintenant : " .. SelShared.get(SAISONHIER))

	if SelShared.get( SAISON ) then
		SubTasks( Tasks['Saison'] )
	end
end

Tasks['Saison']={}
table.insert( Topics, { topic = SAISON, trigger=chgSaison, trigger_once=true } )
table.insert( Topics, { topic = SAISONHIER, trigger=chgSaisonHier, trigger_once=true } )
