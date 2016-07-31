-- Kitchen

--
-- Topics
--

local TCUISINE='maison/Volet/Cuisine/Fenetre'

-- Plage durant laquelle les volets peuvent se baisser
-- automatiquement pour conserver de la fraîcheur
-- Format hh.mm
local HDebFraicheurCuisineAuto=9
local HFinFraicheurCuisineAuto=17.3
-- Température à partir de laquelle les volets se fermeront
local LimitTemperature=22

--
-- Actions
--

function OuvreCuisine()
	VoletOuvre(TCUISINE, 'Cuisine')
end

function FermeCuisine()
	VoletFerme(TCUISINE, 'Cuisine')
end

function MyCuisine()
	VoletMy(TCUISINE, 'Cuisine')
end

--
-- Triggers
--

function determineCuisine()
	SelLog.log("Détermination du planning pour la cuisine")

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		if SelShared.get(MODE) == 'Absent' then
			ColAddFunc( Tasks['SLeve'], OuvreCuisine)
			SelLog.log("Ouverture de la cuisine avec le soleil")
		else
			local h = 8.15	-- Ouverture par defaut
			if SelShared.get(MODE) == 'Travail' then
				h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
			end
			tmrAddEntry( tbl_timers, h, OuvreCuisine)
			SelLog.log("Ouverture de la Cuisine à " .. h)
		end

			-- Conservation de la fraîcheur
		local dt = os.date("*t")
		local cur = dt.hour + dt.min/100

		if cur < HDebFraicheurCuisineAuto then	-- avant la période de détermination
			SelLog.log("La température de la cuisine sera surveillée à partir de ".. HDebFraicheurCuisineAuto)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheurCuisineAuto)
			
			tmrAddEntry( tbl_timers, HDebFraicheurCuisineAuto, LanceFraicheurCuisineAuto)
			tmrAddEntry( tbl_timers, HFinFraicheurCuisineAuto, FinFraicheurCuisineAuto)
			ColRemoveFunc( Tasks['TSalon'], FraicheurCuisineAuto)
		elseif cur < HFinFraicheurCuisineAuto then -- pendant
			LanceFraicheurCuisineAuto()
			tmrAddEntry( tbl_timers, HFinFraicheurCuisineAuto, FinFraicheurCuisineAuto)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheurCuisineAuto)
		else	-- après
			FinFraicheurCuisineAuto()
		end
	end

	ColAddFunc( Tasks['SCouche'], FermeCuisine)
	SelLog.log("Fermeture de la Cuisine au couché du soleil")

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determineCuisine )
table.insert( Tasks['Mode'], determineCuisine )

--
-- Gestion des températures
-- 

function FraicheurCuisineAuto()
	if SelShared.get( TSalon ) > LimitTemperature then
		SelLog.log("TSalon : " .. SelShared.get( TSalon ) .. ", les volets se ferment")
		MyCuisine()
		FinFraicheurCuisineAuto()
		tmrAddEntry( tbl_timers, HFinFraicheurCuisineAuto, OuvreCuisine)
		SelLog.log("Le volet de la cuisine s'ouvrira à : ".. HFinFraicheurCuisineAuto)
	end
end

function LanceFraicheurCuisineAuto()
	SelLog.log("Début de la surveillance de la température de la cuisine")
	
	tmrRemoveEntry(tbl_timers, LanceFraicheurCuisineAuto)
	ColAddFunc( Tasks['TSalon'], FraicheurCuisineAuto)
end

function FinFraicheurCuisineAuto()
	SelLog.log("Fin de la surveillance de la température de la cuisine")

	tmrRemoveEntry(tbl_timers, LanceFraicheurCuisineAuto)
	tmrRemoveEntry(tbl_timers, FinFraicheurCuisineAuto)
	ColRemoveFunc( Tasks['TSalon'], FraicheurCuisineAuto)
end

