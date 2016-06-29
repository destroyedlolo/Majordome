-- Living

--
-- Topics
--

local TBalcon='maison/Volet/Salon/Balcon'
local TFenetre='maison/Volet/Salon/Fenetre'
local TCheminee='maison/Volet/Salon/Cheminee'

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
	SelLog.log("Détermination du planning pour le salon")

		-- Ménage nécessaire car peut-être avons changé de saison
	ColRemoveFunc(Tasks['SCouche'], MySalon)
	ColRemoveFunc(Tasks['SCouche'], FermeSalon)

	if SelShared.get( SAISON ) == 'Hiver' then
	elseif SelShared.get( SAISON ) == 'Ete' then
			-- En mode été, il est possible que les fenêtres soient restées
			-- ouverte en My : il ne faut donc pas les ouvrir en plein.
		tmrRemoveEntry(tbl_timers, OuvreSalon)
		SelLog.log("Pas d'ouverture du salon car nous sommes en été")

		ColAddFunc( Tasks['SCouche'], MySalon)
		SelLog.log("Fermeture My du Salon au couché du soleil")
	else
		local h = 8.15	-- Ouverture par defaut
		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
		end
		tmrAddEntry( tbl_timers, h, OuvreSalon)
		SelLog.log("Ouverture du Salon à " .. h)

		ColAddFunc( Tasks['SCouche'], FermeSalon)
		SelLog.log("Fermeture du Salon au couché du soleil")
	end

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determineSalon )
table.insert( Tasks['Mode'], determineSalon )


