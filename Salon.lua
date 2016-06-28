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

	if SelShared.get( SAISON ) == 'Hiver' then
	else
		local h = 8.15	-- Ouverture par defaut
		if SelShared.get(MODE) == 'Travail' then
			h = DEC2DMS(DMS2DEC(SelShared.get( HLEVE )) - DMS2DEC(0.15))
		end
		tmrAddEntry( tbl_timers, h, OuvreSalon)
		SelLog.log("Ouverture du Salon à " .. h)
	end

	ColAddFunc( Tasks['SCouche'], FermeSalon)
	SelLog.log("Fermeture du Salon au couché du soleil")

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determineSalon )
table.insert( Tasks['Mode'], determineSalon )
table.insert( Tasks['SCouche'], determineSalon )

