-- 1st floor

--
-- Topics
--

local TpChOceane='maison/Volet/chOceane'
local TpChJoris='maison/Volet/chJoris'
local TpChParents='maison/Volet/chParents'

-- Plage durant laquelle les volets peuvent se baisser
-- automatiquement pour conserver de la fraîcheur
-- Format hh.mm
local HDebFraicheurAutoV=11	-- Auto pour les vacances
local HDebFraicheurOJAutoT=9.3
local HDebFraicheurParentT=8.4
local HFinFraicheur1erAuto=17.3

-- Température à partir de laquelle les volets se fermeront
local LimiteTemperature=21

--
-- Actions
--

function OuvreChOceane()
	VoletOuvre(TpChOceane, 'Chambre Océane')
end

function FermeChOceane()
	VoletFerme(TpChOceane, 'Chambre Océane')
end

function MyChOceane()
	VoletMy(TpChOceane, 'Chambre Océane')
end

function OuvreChJoris()
	VoletOuvre(TpChJoris, 'Chambre Joris')
end

function FermeChJoris()
	VoletFerme(TpChJoris, 'Chambre Joris')
end

function MyChJoris()
	VoletMy(TpChJoris, 'Chambre Joris')
end

function OuvreChParents()
	VoletOuvre(TpChParents, 'Chambre Parents')
end

function FermeChParents()
	VoletFerme(TpChParents, 'Chambre Parents')
end

function MyChParents()
	VoletMy(TpChParents, 'Chambre Parents')
end

--
-- Triggers
--

function determinePlanning1er()
	SelLog.log("Détermination du planning pour les chambres du 1er")

	if SelShared.get(MODE) == 'Absent' then
		SelLog.log("Rien à faire : nous ne sommes pas là")
		tmrRemoveEntry(tbl_timers, MyChOceane)
		tmrRemoveEntry(tbl_timers, MyChJoris)
		tmrRemoveEntry(tbl_timers, MyChParents)

		tmrRemoveEntry(tbl_timers, OuvreChJoris)
		tmrRemoveEntry(tbl_timers, OuvreChOceane)
		tmrRemoveEntry(tbl_timers, OuvreChParents)
		return
	end

	local h
		-- Ouverture seulement si on bosse
	if SelShared.get(MODE) == 'Travail' then
		if SelShared.get(MODEENFANTS) == 'Vacances' then
			SelLog.log("Les enfants sont en mode vacances ... pas d'ouverture")
			tmrRemoveEntry(tbl_timers, MyChOceane)
			tmrRemoveEntry(tbl_timers, MyChJoris)
			tmrRemoveEntry(tbl_timers, OuvreChJoris)
			tmrRemoveEntry(tbl_timers, OuvreChOceane)
		else
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.1))
			tmrAddEntry( tbl_timers, h, MyChOceane )
			tmrAddEntry( tbl_timers, h, MyChJoris )
			SelLog.log("Ouverture 'My' des chambres des enfants à " .. h)
		
			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), OuvreChJoris )
			tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), OuvreChOceane )
			SelLog.log("Ouverture des chambres des enfants à " .. SelShared.get( HLEVE ))
		end

		tmrAddEntry( tbl_timers, SelShared.get( HLEVE ), MyChParents )
		SelLog.log("Ouverture 'My' de la chambre des parents à " .. SelShared.get( HLEVE ))
		h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) + DMS2DEC(0.05))
		tmrAddEntry( tbl_timers, h, OuvreChParents )
		SelLog.log("Ouverture de la chambre des parents à " .. h)
	else
		tmrRemoveEntry(tbl_timers, MyChOceane)
		tmrRemoveEntry(tbl_timers, MyChJoris)
		tmrRemoveEntry(tbl_timers, MyChParents)

		tmrRemoveEntry(tbl_timers, OuvreChJoris)
		tmrRemoveEntry(tbl_timers, OuvreChOceane)
		tmrRemoveEntry(tbl_timers, OuvreChParents)
	end

			-- Fermeture le soir
	local t = SelShared.get(TSunSet) or 18.0
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
		SelLog.log("Fermeture des chambres du haut à " .. t)
	end

			-- Conservation de la fraîcheur
	if SelShared.get( SAISON ) == 'Hiver' then
		tmrRemoveEntry(tbl_timers, LanceFraicheurChOJAuto)
		tmrRemoveEntry(tbl_timers, FinFraicheurChOJAuto)
		ColRemoveFunc( Tasks['TChOceane'], FraicheurChOJAuto)

		tmrRemoveEntry(tbl_timers, LanceFraicheurChPAuto)
		tmrRemoveEntry(tbl_timers, FinFraicheurChPAuto)
		ColRemoveFunc( Tasks['TChOceane'], FraicheurChPAuto)
	else
		local dt = os.date("*t")
		local cur = dt.hour + dt.min/100

			-- Pour les enfants
		local hdebut = (SelShared.get(MODE) == 'Travail' and SelShared.get(MODEENFANTS) ~= 'Vacances') and HDebFraicheurOJAutoT or HDebFraicheurAutoV
		if cur < hdebut then	-- avant
			SelLog.log("La température des chambres des enfants sera surveillée à partir de ".. hdebut)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheur1erAuto)

			tmrAddEntry( tbl_timers, hdebut, LanceFraicheurChOJAuto)
			tmrAddEntry( tbl_timers, HFinFraicheur1erAuto, FinFraicheurChOJAuto)
			ColRemoveFunc( Tasks['TChOceane'], FraicheurChOJAuto)
		elseif cur < HFinFraicheur1erAuto then	-- pendant
			LanceFraicheurChOJAuto()
			tmrAddEntry( tbl_timers, HFinFraicheur1erAuto, FinFraicheurChOJAuto)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheur1erAuto)
		else -- apres
			FinFraicheurChOJAuto()
		end

			-- Pour les parents
		local hdebut = (SelShared.get(MODE) == 'Travail') and HDebFraicheurParentT or HDebFraicheurAutoV
		if cur < hdebut then	-- avant
			SelLog.log("La température de la chambre des parents sera surveillée à partir de ".. hdebut)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheur1erAuto)

			tmrAddEntry( tbl_timers, hdebut, LanceFraicheurChPAuto)
			tmrAddEntry( tbl_timers, HFinFraicheur1erAuto, FinFraicheurChPAuto)
			ColRemoveFunc( Tasks['TChOceane'], FraicheurChPAuto)
		elseif cur < HFinFraicheur1erAuto then	-- pendant
			LanceFraicheurChPAuto()
			tmrAddEntry( tbl_timers, HFinFraicheur1erAuto, FinFraicheurChPAuto)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheur1erAuto)
		else -- apres
			FinFraicheurChOJAuto()
		end
	end

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determinePlanning1er )
table.insert( Tasks['Mode'], determinePlanning1er )
table.insert( Tasks['ModeEnfants'], determinePlanning1er )
table.insert( Tasks['Consignes'], determinePlanning1er )

--
-- Gestion des températures
-- 

function FraicheurChOJAuto()
	if SelShared.get( TChOceane ) > LimiteTemperature then
		SelLog.log("TChOceane : " .. SelShared.get( TChOceane ) .. ", les volets se ferment")
		MyChOceane()
		MyChJoris()
		FinFraicheurChOJAuto()
	end
end

function LanceFraicheurChOJAuto()
	SelLog.log("Début de la surveillance de la température des chambres des enfants")
	
	tmrRemoveEntry(tbl_timers, LanceFraicheurChOJAuto)
	ColAddFunc( Tasks['TChOceane'], FraicheurChOJAuto)
end

function FinFraicheurChOJAuto()
	SelLog.log("Fin de la surveillance de la température des chambres des enfants")

	tmrRemoveEntry(tbl_timers, LanceFraicheurChOJAuto)
	tmrRemoveEntry(tbl_timers, FinFraicheurChOJAuto)
	ColRemoveFunc( Tasks['TChOceane'], FraicheurChOJAuto)
end

function FraicheurChPAuto()
	if SelShared.get( TChOceane ) > LimiteTemperature then
		SelLog.log("TChOceane : " .. SelShared.get( TChOceane ) .. ", les volets se ferment")
		MyChParents()
		FinFraicheurChPAuto()
	end
end

function LanceFraicheurChPAuto()
	SelLog.log("Début de la surveillance de la température de la chambres des parents")
	
	tmrRemoveEntry(tbl_timers, LanceFraicheurChPAuto)
	ColAddFunc( Tasks['TChOceane'], FraicheurChPAuto)
end

function FinFraicheurChPAuto()
	SelLog.log("Fin de la surveillance de la température de la chambre des parents")

	tmrRemoveEntry(tbl_timers, LanceFraicheurChPAuto)
	tmrRemoveEntry(tbl_timers, FinFraicheurChPAuto)
	ColRemoveFunc( Tasks['TChOceane'], FraicheurChPAuto)
end

