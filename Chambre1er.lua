-- 1st floor

--
-- Topics
--

local TChOceane='maison/Volet/chOceane'
local TChJoris='maison/Volet/chJoris'
local TChParents='maison/Volet/chParents'

--
-- Actions
--

function OuvreChOceane()
	VoletOuvre(TChOceane, 'Chambre Océane')
end

function FermeChOceane()
	VoletFerme(TChOceane, 'Chambre Océane')
end

function MyChOceane()
	VoletMy(TChOceane, 'Chambre Océane')
end

function OuvreChJoris()
	VoletOuvre(TChJoris, 'Chambre Joris')
end

function FermeChJoris()
	VoletFerme(TChJoris, 'Chambre Joris')
end

function MyChJoris()
	VoletMy(TChJoris, 'Chambre Joris')
end

function OuvreChParents()
	VoletOuvre(TChParents, 'Chambre Parents')
end

function FermeChParents()
	VoletFerme(TChParents, 'Chambre Parents')
end

function MyChParents()
	VoletMy(TChParents, 'Chambre Parents')
end

--
-- Triggers
--

function determinePlanning1er()
	SelLog.log("Détermination du planning pour les chambres du 1er")

	if SelShared.get( SAISON ) == 'Hiver' then
	else
print("*** D *** ok")
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

		local h
			-- Ouverture seulement si on bosse
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

		local t = SelShared.get(TSunSet)
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
		else
			tmrAddEntry( tbl_timers, t, FermeVoletChJoris )
			tmrAddEntry( tbl_timers, t, FermeVoletChOceane )
			tmrAddEntry( tbl_timers, t, FermeVoletChParents )
			SelLog.log("Fermeture des chambres du haut à " .. h)
		end
	end
end

table.insert( Tasks['Saison'], determinePlanning1er )
table.insert( Tasks['Mode'], determinePlanning1er )
table.insert( Tasks['Consignes'], determinePlanning1er )
