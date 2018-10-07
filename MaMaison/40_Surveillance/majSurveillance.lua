-- Détermination de l'heure de fin de surveillance en fonction du soleil
-->> listen=CoucherSoleil
-->> listen=Saison

--
-- Vérification que tous les paramètres sont bien présents
--

if not SelShared.Get("CoucherSoleil")
or not SelShared.Get("Saison") then
	return
end

--
-- Récupération des infos
--

local DebutSurveillance = MajordomeTimer.find("DebutSurveillance")
assert(DebutSurveillance, "'DebutSurveillance' pas trouvé")

local FinSurveillance = MajordomeTimer.find("FinSurveillance")
assert(FinSurveillance, "'FinSurveillance' pas trouvé")

local notifRecalcul = MajordomeRendezVous.find("recalculeSurveillance")
assert(notifRecalcul, "'recalculeSurveillance' pas trouvé")


local hc,mc = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Saison") == 'Hiver' then	-- L'hiver il n'y a pas de surveillance
	SelLog.log('I', "C'est l'hiver : Pas de surveillance de la température")

	if DebutSurveillance:isEnabled() then
		DebutSurveillance:Disable()
	end
	if FinSurveillance:isEnabled() then
		FinSurveillance:Disable()
	end
else	-- Autre saison : on surveille
	hc = hc - 2;	-- On arrête la surveillance 2 heure avant le coucher du soleil
	if hc > 16 then	-- on force la fin de surveillance à 17:00 si le soleil se couche tard
		hc = 17
		mc = 0
	end
	FinSurveillance:setAtHM( hc, mc )
	hc,mc = FinSurveillance:getAtHM()
	SelLog.log('I', "La surveillance de température se termine à ".. hc ..":".. mc)
end

notifRecalcul:Launch()
