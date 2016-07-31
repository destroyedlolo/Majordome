-- Living

--
-- Topics
--

local TBalcon='maison/Volet/Salon/Balcon'
local TFenetre='maison/Volet/Salon/Fenetre'
local TCheminee='maison/Volet/Salon/Cheminee'

-- Plage durant laquelle les volets peuvent se baisser
-- automatiquement pour conserver de la fraîcheur
-- Format hh.mm
local HDebFraicheurSalonAuto=10
local HFinFraicheurSalonAuto=17.3
-- Température à partir de laquelle les volets se fermeront
local LimitTemperature=22

--
-- Actions
--

function OuvreSalon()
	VoletOuvre(TBalcon, 'Balcon')
	VoletOuvre(TFenetre, 'Fenêtre du salon')
	VoletOuvre(TCheminee, 'Cheminée')
end

function FermeSalon()
	VoletFerme(TBalcon, 'Balcon')
	VoletFerme(TFenetre, 'Fenêtre du salon')
	VoletFerme(TCheminee, 'Cheminée')
end

function MySalon()
	VoletMy(TBalcon, 'Balcon')
	VoletMy(TFenetre, 'Fenêtre du salon')
	VoletMy(TCheminee, 'Cheminée')
end

--
-- Triggers
--

function determineSalon()
	local h
	SelLog.log("Détermination du planning pour le salon")

		-- Ménage nécessaire car peut-être avons changé de saison
	ColRemoveFunc(Tasks['SCouche'], MySalon)
	ColRemoveFunc(Tasks['SCouche'], FermeSalon)

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		if SelShared.get(MODE) == 'Absent' then
			ColAddFunc( Tasks['SLeve'], OuvreSalon)
			SelLog.log("Ouverture du salon avec le soleil")
			ColAddFunc( Tasks['SCouche'], FermeSalon)
			SelLog.log("Fermeture du Salon au couché du soleil")
		else
				-- Ouverture le matin
			if SelShared.get( SAISONHIER ) == 'Ete' then
					-- En mode été, il est possible que les fenêtres soient restées
					-- ouverte en My : il ne faut donc pas les ouvrir en plein.
				tmrRemoveEntry(tbl_timers, OuvreSalon)
				SelLog.log("Pas d'ouverture du salon car nous sommes en été")
			else
				h = 8.15	-- Ouverture par defaut
				if SelShared.get(MODE) == 'Travail' then
					h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
				end
				tmrAddEntry( tbl_timers, h, OuvreSalon)
				SelLog.log("Ouverture du Salon à " .. h)
			end

				-- Fermeture le soir
			if SelShared.get( SAISONHIER ) == 'Ete' then
				ColRemoveFunc( Tasks['SCouche'], FermeSalon )
				ColAddFunc( Tasks['SCouche'], MySalon)
				SelLog.log("Fermeture My du Salon au couché du soleil")
			else
				ColRemoveFunc( Tasks['SCouche'], MySalon )
				ColAddFunc( Tasks['SCouche'], FermeSalon)
				SelLog.log("Fermeture du Salon au couché du soleil")
			end
		end

			-- Conservation de la fraîcheur
		local dt = os.date("*t")
		local cur = dt.hour + dt.min/100

		if cur < HDebFraicheurSalonAuto then	-- avant la période de détermination
			SelLog.log("La température du salon sera surveillée à partir de ".. HDebFraicheurSalonAuto)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheurSalonAuto)

			tmrAddEntry( tbl_timers, HDebFraicheurSalonAuto, LanceFraicheurSalonAuto)
			tmrAddEntry( tbl_timers, HFinFraicheurSalonAuto, FinFraicheurSalonAuto)
			ColRemoveFunc( Tasks['TSalon'], FraicheurSalonAuto)
		elseif cur < HFinFraicheurSalonAuto then -- pendant
			LanceFraicheurSalonAuto()
			tmrAddEntry( tbl_timers, HFinFraicheurSalonAuto, FinFraicheurSalonAuto)
			SelLog.log("La surveillance se terminera à ".. HFinFraicheurSalonAuto)
		else	-- après
			FinFraicheurSalonAuto()
		end
	end

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determineSalon )
table.insert( Tasks['Mode'], determineSalon )

--
-- Gestion des températures
-- 

function FraicheurSalonAuto()
	if SelShared.get( TSalon ) > LimitTemperature then
		SelLog.log("TSalon : " .. SelShared.get( TSalon ) .. ", les volets se ferment")
		MySalon()
		FinFraicheurSalonAuto()
		tmrAddEntry( tbl_timers, HFinFraicheurSalonAuto, OuvreSalon)
		SelLog.log("Le volet du salon s'ouvrira à : ".. HFinFraicheurSalonAuto)
	end
end

function LanceFraicheurSalonAuto()
	SelLog.log("Début de la surveillance de la température du salon")
	
	tmrRemoveEntry(tbl_timers, LanceFraicheurSalonAuto)
	ColAddFunc( Tasks['TSalon'], FraicheurSalonAuto)
end

function FinFraicheurSalonAuto()
	SelLog.log("Fin de la surveillance de la température du salon")

	tmrRemoveEntry(tbl_timers, LanceFraicheurSalonAuto)
	tmrRemoveEntry(tbl_timers, FinFraicheurSalonAuto)
	ColRemoveFunc( Tasks['TSalon'], FraicheurSalonAuto)
end

