-- Kids bedrooms

-- Actions

function FermeVoletChOceane()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture du volet de la chambre d'Océane annulée")
	else
		SelLog.log("Fermeture du volet de la chambre d'Océane")

		Brk:Publish( 'maison/Volet/chOceane', 'Down' )
	end
end

function FermeVoletChJoris()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture du volet de la chambre de Joris annulée")
	else
		SelLog.log("Fermeture du volet de la chambre de Joris")

		Brk:Publish( 'maison/Volet/chJoris', 'Down' )
	end
end

function FermeVoletChParents()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture du volet de la chambre des Parents annulée")
	else
		SelLog.log("Fermeture du volet de la chambre des Parents")

		Brk:Publish( 'maison/Volet/chParents', 'Down' )
	end
end

function OuvreVoletChOceane()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture du volet de la chambre d'Océane annulée")
	else
		SelLog.log("Ouverture du volet de la chambre d'Océane")

		Brk:Publish( 'maison/Volet/chOceane', 'Up' )
	end
end

function OuvreVoletChJoris()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture du volet de la chambre de Joris annulée")
	else
		SelLog.log("Ouverture du volet de la chambre de Joris")

		Brk:Publish( 'maison/Volet/chJoris', 'Up' )
	end
end

function OuvreVoletChParents()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture du volet de la chambre des Parents annulée")
	else
		SelLog.log("Ouverture du volet de la chambre des Parents")

		Brk:Publish( 'maison/Volet/chParents', 'Up' )
	end
end

function MyVoletChOceane()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' du volet de la chambre d'Océane annulée")
	else
		SelLog.log("Ouverture 'My' du volet de la chambre d'Océane")

		Brk:Publish( 'maison/Volet/chOceane', 'My' )
	end
end

function MyVoletChJoris()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' du volet de la chambre de Joris annulée")
	else
		SelLog.log("Ouverture 'My' du volet de la chambre de Joris")

		Brk:Publish( 'maison/Volet/chJoris', 'My' )
	end
end

function MyVoletChParents()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' du volet de la chambre des Parents annulée")
	else
		SelLog.log("Ouverture 'My' du volet de la chambre des Parents")

		Brk:Publish( 'maison/Volet/chParents', 'My' )
	end
end

--
-- triggers
--

function determinePlanning1er()
	SelLog.log("Détermination du planning pour les chambres du 1er")

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		local h
		-- cleaning
		tmrRemoveEntry(tbl_timers, MyVoletChJoris)
		tmrRemoveEntry(tbl_timers, MyVoletChOceane)
		tmrRemoveEntry(tbl_timers, OuvreVoletChJoris)
		tmrRemoveEntry(tbl_timers, OuvreVoletChOceane)
		tmrRemoveEntry(tbl_timers, FermeVoletChJoris)
		tmrRemoveEntry(tbl_timers, FermeVoletChOceane)
		tmrRemoveEntry(tbl_timers, MyVoletChParents)
		tmrRemoveEntry(tbl_timers, OuvreVoletChParents)
		tmrRemoveEntry(tbl_timers, FermeVoletChParents)

		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.1))
			tmrAddEntry( tbl_timers, h, MyVoletChJoris )
			tmrAddEntry( tbl_timers, h, MyVoletChOceane )
			SelLog.log("Ouverture 'My' des chambres des enfants à " .. h)
		
			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), OuvreVoletChJoris )
			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), OuvreVoletChOceane )
			SelLog.log("Ouverture des chambres des enfants à " .. SelShared.get( HLEVE ))

			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), MyVoletChParents )
			SelLog.log("Ouverture 'My' de la chambre des parents à " .. SelShared.get( HLEVE ))
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) + DMS2DEC(0.05))
			tmrAddEntry( tbl_timers, h, OuvreVoletChParents )
			SelLog.log("Ouverture de la chambre des parents à " .. h)
		end

		local t = TXT2DMS( SelShared.get(METEO .. '/sunset') )
		h = DEC2DMS(DMS2DEC(SelShared.get( HCOUCHE )) - DMS2DEC(0.15))
		if t > h then
			tmrAddEntry( tbl_timers, h, MyVoletChJoris )
			tmrAddEntry( tbl_timers, h, MyVoletChOceane )
			SelLog.log("Fermeture 'My' des chambres des enfants à " .. h)

			h = SelShared.get( HCOUCHE )
			tmrAddEntry( tbl_timers, h, FermeVoletChJoris )
			tmrAddEntry( tbl_timers, h, FermeVoletChOceane )
			tmrAddEntry( tbl_timers, h, FermeVoletChParents )
			SelLog.log("Fermeture des chambres du haut à " .. h)

		end
	end
	tmrRethink( timerCron, tbl_timers );

SelLog.log("Le planing done :" .. universal_tostring( tbl_timers ))
end

table.insert( Saison_tasks, determinePlanning1er )

