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
		local h
			-- Ouverture seulement si on bosse
		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.1))
			tmrAddEntry( tbl_timers, h, MyChOceane )
			tmrAddEntry( tbl_timers, h, MyChJoris )
			SelLog.log("Ouverture 'My' des chambres des enfants à " .. h)
		
			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), OuvreChJoris )
			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), OuvreChOceane )
			SelLog.log("Ouverture des chambres des enfants à " .. SelShared.get( HLEVE ))

			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), MyChParents )
			SelLog.log("Ouverture 'My' de la chambre des parents à " .. SelShared.get( HLEVE ))
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) + DMS2DEC(0.05))
			tmrAddEntry( tbl_timers, h, OuvreChParents )
			SelLog.log("Ouverture de la chambre des parents à " .. h)
		end

		local t = SelShared.get(TSunSet)
		h = DEC2DMS(DMS2DEC(SelShared.get( HCOUCHE )) - DMS2DEC(0.15))
		if t > h then
			tmrAddEntry( tbl_timers, h, MyChJoris )
			tmrAddEntry( tbl_timers, h, MyChOceane )
			SelLog.log("Fermeture 'My' des chambres des enfants à " .. h)

			h = SelShared.get( HCOUCHE )
			tmrAddEntry( tbl_timers, h, FermeChJoris )
			tmrAddEntry( tbl_timers, h, FermeChOceane )
			tmrAddEntry( tbl_timers, h, FermeChParents )
			SelLog.log("Fermeture des chambres du haut à " .. h)
		else
			tmrAddEntry( tbl_timers, t, FermeChJoris )
			tmrAddEntry( tbl_timers, t, FermeChOceane )
			tmrAddEntry( tbl_timers, t, FermeChParents )
			SelLog.log("Fermeture des chambres du haut à " .. h)
		end
	end

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determinePlanning1er )
table.insert( Tasks['Mode'], determinePlanning1er )
table.insert( Tasks['Consignes'], determinePlanning1er )
