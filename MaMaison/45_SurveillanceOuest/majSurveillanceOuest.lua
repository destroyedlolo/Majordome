-- Détermination de l'heure de fin de surveillance en fonction du soleil
-- L'heure de fin est gérée par les surveillances générales
-->> listen=Saison

--
-- Récupération des infos
--

local DebutSurveillanceOuest = MajordomeTimer.find("DebutSurveillanceOuest", true)
local notifRecalcul = MajordomeRendezVous.find("recalculeSurveillanceOuest", true)

if SelShared.Get("Saison") == 'Hiver' then	-- L'hiver il n'y a pas de surveillance
	SelLog.log('I', "C'est l'hiver : Pas de surveillance de la température (Ouest)")

	if DebutSurveillanceOuest:isEnabled() then
		DebutSurveillanceOuest:Disable()
	end

	local rdv = MajordomeRendezVous.find("desactiveSurveillanceOuest", true)
	rdv:Launch()
else	-- Autre saison : on surveille
	if not DebutSurveillanceOuest:isEnabled() then
		DebutSurveillanceOuest:Enable()
	end

	local rdv = MajordomeRendezVous.find("activeSurveillanceOuest", true)
	rdv:Launch()

end

notifRecalcul:Launch()
