-- Office and guest room

--
-- Topics
--

local TpBuro='maison/Volet/Buro'
local TpChAmis='maison/Volet/chAmis'

-- Plage durant laquelle les volets peuvent se baisser
-- automatiquement pour conserver de la fraîcheur
-- Format hh.mm
local HDebFraicheurAuto=9.3
local HFinFraicheurAuto=16.3

-- Température à partir de laquelle les volets se fermeront
local LimiteTemperature=21.5

--
-- Actions
--

function OuvreBuro()
	VoletOuvre(TpBuro, 'Bureau')
end

function FermeBuro()
	VoletFerme(TpBuro, 'Bureau')
end

function MyBuro()
	VoletMy(TpBuro, 'Bureau')
end

function OuvreChAmis()
	VoletOuvre(TpChAmis, 'Chambre Amis')
end

function FermeChAmis()
	VoletFerme(TpChAmis, 'Chambre Amis')
end

function MyChAmis()
	VoletMy(TpChAmis, 'Chambre Amis')
end

--
-- Triggers
--

function determinePlanningRdC()
	SelLog.log("Détermination du planning pour le rez de chaussée")

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		local h = 8.15	-- Ouverture par defaut
		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
		end
		tmrAddEntry( tbl_timers, h, OuvreBuro)
		tmrAddEntry( tbl_timers, h, OuvreChAmis)
		SelLog.log("Ouverture de la chambre d'amis et du bureau à " .. h)

		if SelShared.get( SAISON ) == 'Ete' then
			tmrAddEntry( tbl_timers, 9, MyChAmis)
			SelLog.log("My de la chambre d'amis à 9.0")
			tmrAddEntry( tbl_timers, 16, OuvreChAmis)
			SelLog.log("Ouverture de la chambre d'amis à 16.0")
		end

			-- Conservation de la fraîcheur
		local dt = os.date("*t")
		local cur = dt.hour + dt.min/100

		if cur < HDebFraicheurAuto then	-- avant
			SelLog.log("La température du bureau sera surveillée à partir de ".. HDebFraicheurAuto);
			SelLog.log("La surveillance se terminera à ".. HFinFraicheurAuto);

			tmrAddEntry( tbl_timers, HDebFraicheurAuto, LanceFraicheurBureauAuto)
			tmrAddEntry( tbl_timers, HFinFraicheurAuto, FinFraicheurBureauAuto)
			ColRemoveFunc( Tasks['TChOceane'], FraicheurBureauAuto)
		elseif cur < HDebFraicheurAuto then	-- pendant
			LanceFraicheurBureauAuto()
			tmrAddEntry( tbl_timers, HFinFraicheurAuto, FinFraicheurBureauAuto)

			SelLog.log("La surveillance se terminera à ".. HFinFraicheurAuto);
		else -- apres
			FinFraicheurBureauAuto()
		end
	end

	ColAddFunc( Tasks['SCouche'], FermeBuro)
	ColAddFunc( Tasks['SCouche'], FermeChAmis)
	SelLog.log("Fermeture de la chambre d'amis et du bureau au couché du soleil")


	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determinePlanningRdC )
table.insert( Tasks['Mode'], determinePlanningRdC )

--
-- Gestion des températures
-- 

function FraicheurBureauAuto()
	if SelShared.get( TChOceane ) > LimiteTemperature then
		SelLog.log("TBureau : " .. SelShared.get( TBureau ) .. ", les volets se ferment")
		MyBuro()
		FinFraicheurBureauAuto()
	end
end

function LanceFraicheurBureauAuto()
	SelLog.log("Début de la surveillance de la température du bureau")
	
	tmrRemoveEntry(tbl_timers, LanceFraicheurBureauAuto)
	ColAddFunc( Tasks['TBureau'], FraicheurBureauAuto)
end

function FinFraicheurBureauAuto()
	SelLog.log("Fin de la surveillance de la température du bureau")

	tmrRemoveEntry(tbl_timers, LanceFraicheurBureauAuto)
	tmrRemoveEntry(tbl_timers, FinFraicheurChPAuto)
	ColRemoveFunc( Tasks['TBureau'], FraicheurBureauAuto)
end

