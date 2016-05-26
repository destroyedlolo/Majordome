-- Living room

--
-- Actions
--

function FermeVoletSalon()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture des volets du Salon annulée")
	else
		SelLog.log("Fermeture des volets du Salon")

		Brk:Publish( 'maison/Volet/Salon/Balcon', 'Down' )
		Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Down' )
		Brk:Publish( 'maison/Volet/Salon/Cheminee', 'Down' )
	end
end

function OuvreVoletSalon()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture des volets du Salon annulée")
	else
		SelLog.log("Ouverture des volets du Salon")

		Brk:Publish( 'maison/Volet/Salon/Fenetre', 'Up' )
		Brk:Publish( 'maison/Volet/Salon/Balcon', 'Up' )
		Brk:Publish( 'maison/Volet/Salon/Cheminee', 'Up' )
	end
end

function MyVoletSalon()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' des volets du Salon annulée")
	else
		SelLog.log("Ouverture 'My' des volets du Salon")

		Brk:Publish( 'maison/Volet/Salon/Fenetre', 'My' )
		Brk:Publish( 'maison/Volet/Salon/Balcon', 'My' )
		Brk:Publish( 'maison/Volet/Salon/Cheminee', 'My' )
	end
end

--
-- triggers
--
function determinePlanningSalon()
	SelLog.log("Détermination du planning pour le salon")

	if SelShared.get( SAISON ) == 'Ete' then
	elseif SelShared.get( SAISON ) == 'Intersaison' then
		tmrRemoveEntry(tbl_timers, OuvreVoletSalon)
		if SelShared.get(MODE) == 'Travail' then
			local h = DEC2DMS(DMS2DEC(SelShared.get( HLEVER )) - DMS2DEC(0.15))
			tmrAddEntry( tbl_timers, h, OuvreVoletSalon )
			SelLog.log("Ouverture du salon à " .. h)
		else
			tmrAddEntry( tbl_timers, 8.15, OuvreVoletSalon )
			SelLog.log("Ouverture du salon à 8h15")
		end
		ColRemoveFunc( Sunset_tasks, FermeVoletSalon )
		ColAddFunc( Sunset_tasks, FermeVoletSalon )
		SelLog.log("Fermeture du salon au coucher du soleil")
	else	-- Hiver
	end
end

table.insert( Saison_tasks, determinePlanningSalon )


