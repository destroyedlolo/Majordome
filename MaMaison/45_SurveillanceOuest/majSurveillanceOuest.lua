-- Détermination de l'heure de fin de surveillance en fonction du soleil
-- L'heure de fin est gérée par les surveillances générales
-->> listen=Saison
-->> need_topic=Saison
-->> need_timer=DebutSurveillanceOuest
-->> need_rendezvous=recalculeSurveillanceOuest
-->> need_rendezvous=desactiveSurveillanceOuest
-->> need_rendezvous=activeSurveillanceOuest

--
-- Récupération des infos
--

if Saison:getVal() == 'Hiver' then	-- L'hiver il n'y a pas de surveillance
	SelLog.Log('I', "C'est l'hiver : Pas de surveillance de la température (Ouest)")

	if DebutSurveillanceOuest:isEnabled() then
		DebutSurveillanceOuest:Disable()
	end

	desactiveSurveillanceOuest:Launch()
else	-- Autre saison : on surveille
	if not DebutSurveillanceOuest:isEnabled() then
		DebutSurveillanceOuest:Enable()
	end

	activeSurveillanceOuest:Launch()
end

recalculeSurveillanceOuest:Launch()
