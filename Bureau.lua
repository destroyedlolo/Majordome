-- Office and guest room

-- Actions

function FermeVoletBureau()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture du volet du Bureau annulée")
	else
		SelLog.log("Fermeture du volet du bureau")

		Brk:Publish( 'maison/Volet/Buro', 'Down' )
		Brk:Publish( 'maison/Volet/chAmis', 'Down' )
	end
end

function OuvreVoletBureau()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture du volet du Bureau annulée")
	else
		SelLog.log("Ouverture du volet du bureau")

		Brk:Publish( 'maison/Volet/Buro', 'Up' )
		Brk:Publish( 'maison/Volet/chAmis', 'Up' )
	end
end

function MyVoletBureau()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' du volet du Bureau annulée")
	else
		SelLog.log("Ouverture 'My' du volet du bureau")

		Brk:Publish( 'maison/Volet/Buro', 'My' )
		Brk:Publish( 'maison/Volet/chAmis', 'My' )
	end
end

--
-- triggers
--
function determinePlanningBureau()
	SelLog.log("Détermination du planning pour le Bureau")

	if SelShared.get( SAISON ) == 'Ete' then
	elseif SelShared.get( SAISON ) == 'Intersaison' then
		tmrRemoveEntry(tbl_timers, OuvreVoletBureau)
		if SelShared.get(MODE) == 'Travail' then
			local h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
			tmrAddEntry( tbl_timers, h, OuvreVoletBureau )
			SelLog.log("Ouverture du bureau à " .. h)
		else
			tmrAddEntry( tbl_timers, 8.15, OuvreVoletBureau )
			SelLog.log("Ouverture du bureau à 8h15")
		end
		ColRemoveFunc( Sunset_tasks, FermeVoletBureau )
		ColAddFunc( Sunset_tasks, FermeVoletBureau )
		SelLog.log("Fermeture du bureau au coucher du soleil")
	else	-- Hiver
	end
	tmrRethink( timerCron, tbl_timers );
end

table.insert( Saison_tasks, determinePlanningBureau )

