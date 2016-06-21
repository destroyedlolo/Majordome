-- Kitchen

-- Actions

function FermeVoletChuisine()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture du volet de la cuisine annulée")
	else
		SelLog.log("Fermeture du volet de la cuisine")
		Brk:Publish( 'maison/Volet/Cuisine/Fenetre', 'Down' )
	end
end

function OuvreVoletChuisine()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture du volet de la cuisine annulée")
	else
		SelLog.log("Ouverture du volet de la cuisine")
		Brk:Publish( 'maison/Volet/Cuisine/Fenetre', 'Up' )
	end
end

function MyVoletChuisine()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : My du volet de la cuisine annulée")
	else
		SelLog.log("Ouverture du volet de la cuisine")
		Brk:Publish( 'maison/Volet/Cuisine/Fenetre', 'Up' )
	end
end

--
-- Triggers
--

function determinePlanningCuisine()
	SelLog.log("Détermination du planning pour le salon")

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		tmrRemoveEntry(tbl_timers, FermeVoletChuisine)
		tmrRemoveEntry(tbl_timers, OuvreVoletChuisine)
		tmrRemoveEntry(tbl_timers, MyVoletChuisine)

		local h = SelShared.get( HLEVE )
		if SelShared.get(MODE) == 'Vacances' then
			h = 8.15
		end
		tmrAddEntry( tbl_timers, h, OuvreVoletChuisine)
		SelLog.log("Ouverture de la cuisine à " .. h)

		if SelShared.get( SAISON ) == 'Ete' then
			tmrAddEntry( tbl_timers, 9.00, MyVoletChuisine)
			SelLog.log("My de la cuisine à 9:00")
		end

		ColAddFunc( Sunset_tasks, FermeVoletChuisine )
		SelLog.log("Fermeture de la cuisine au coucher du soleil")
	end
	tmrRethink( timerCron, tbl_timers );
end

table.insert( Saison_tasks, determinePlanningCuisine )
