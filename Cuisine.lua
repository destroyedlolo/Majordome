-- Kitchen

--
-- Topics
--

local TCUISINE='maison/Volet/Cuisine/Fenetre'

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
		local h = 8.15	-- Ouverture par defaut
		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
		end
		tmrAddEntry( tbl_timers, h, OuvreCuisine)
		SelLog.log("Ouverture de la Cuisine à " .. h)
	end

	ColAddFunc( Tasks['SCouche'], FermeCuisine)
	SelLog.log("Fermeture de la Cuisine au couché du soleil")

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determineCuisine )
table.insert( Tasks['Mode'], determineCuisine )

