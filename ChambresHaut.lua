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

		Brk:Publish( 'maison/Volet/chOceane', 'Up' )
	end
end

function MyVoletChJoris()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' du volet de la chambre de Joris annulée")
	else
		SelLog.log("Ouverture 'My' du volet de la chambre de Joris")

		Brk:Publish( 'maison/Volet/chJoris', 'Up' )
	end
end

function MyVoletChParents()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture 'My' du volet de la chambre des Parents annulée")
	else
		SelLog.log("Ouverture 'My' du volet de la chambre des Parents")

		Brk:Publish( 'maison/Volet/chParents', 'Up' )
	end
end

--
-- triggers
--

function determinePlanning1er()
	SelLog.log("Détermination du planning pour les chambres du 1er")

	if SelShared.get( SAISON ) == 'Ete' then
	elseif SelShared.get( SAISON ) == 'Intersaison' then
		-- Chambres des enfants
		local h
		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVER )) - DMS2DEC(0.15))
			tmrAddEntry( tbl_timers, h, MyVoletChJoris )
			tmrAddEntry( tbl_timers, h, MyVoletChOceane )
			SelLog.log("Ouverture 'My' des chambres des enfants à " .. h)
		
			tmrAddEntry( tbl_timers, SelShared.get( HLEVER ), OuvreVoletChJoris )
			tmrAddEntry( tbl_timers, SelShared.get( HLEVER ), OuvreVoletChOceane )
			SelLog.log("Ouverture des chambres des enfants à " .. SelShared.get( HLEVER ))

			tmrAddEntry( tbl_timers, SelShared.get( HLEVER ), MyVoletChParents )
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVER )) + DMS2DEC(0.05))
			tmrAddEntry( tbl_timers, SelShared.get( HLEVER ), OuvreVoletChParents )
			SelLog.log("Ouverture de la chambre des parents à " .. h)
		end

		local t = TXT2DMS( SelShared.get(METEO .. '/sunset') )
		h = DEC2DMS(DMS2DEC(SelShared.get( HCOUCHER )) - DMS2DEC(0.15))
		if t > h then
			tmrAddEntry( tbl_timers, h, MyVoletChJoris )
			tmrAddEntry( tbl_timers, h, MyVoletChOceane )
			SelLog.log("Fermeture 'My' des chambres des enfants à " .. h)

			h = SelShared.get( HCOUCHER )
			tmrAddEntry( tbl_timers, h, FermeVoletChJoris )
			tmrAddEntry( tbl_timers, h, FermeVoletChOceane )
			tmrAddEntry( tbl_timers, h, FermeVoletChParents )
			SelLog.log("Fermeture des chambres du haut à " .. h)

		end
	else	-- Hiver
	end
end

table.insert( Saison_tasks, determinePlanning1er )

