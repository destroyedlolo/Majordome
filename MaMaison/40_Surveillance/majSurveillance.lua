-- Détermination de l'heure de fin de surveillance en fonction du soleil
-->> listen=ConsigneMeteoCoucherSoleil
-->> listen=Saison

--
-- Vérification que tous les paramètres sont bien présents
--

if not SelShared.Get("ConsigneMeteoCoucherSoleil")
or not SelShared.Get("Saison") then
	return
end

--
-- Récupération des infos
--

local DebutSurveillance = MajordomeTimer.find("DebutSurveillance", true)
local FinSurveillance = MajordomeTimer.find("FinSurveillance", true)
local notifRecalcul = MajordomeRendezVous.find("recalculeSurveillance", true)

local hc,mc = string.match(string.gsub( SelShared.Get('ConsigneMeteoCoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Saison") == 'Hiver' then	-- L'hiver il n'y a pas de surveillance
	SelLog.log('I', "C'est l'hiver : Pas de surveillance de la température")

	if DebutSurveillance:isEnabled() then
		DebutSurveillance:Disable()
	end
	if FinSurveillance:isEnabled() then
		FinSurveillance:Disable()
	end

	local rdv = MajordomeRendezVous.find("desactiveSurveillance", true)
	rdv:Launch()
else	-- Autre saison : on surveille
	if not DebutSurveillance:isEnabled() then
		DebutSurveillance:Enable()
	end
	if not FinSurveillance:isEnabled() then
		FinSurveillance:Enable()
	end

	local rdv = MajordomeRendezVous.find("activeSurveillance", true)
	rdv:Launch()

	hc = hc - 2;	-- On arrête la surveillance 2 heure avant le coucher du soleil
--[[
	if hc > 16 then	-- on force la fin de surveillance à 17:00 si le soleil se couche tard
		hc = 17
		mc = 0
	end
--]]
	FinSurveillance:setAtHM( hc, mc )
	hc,mc = FinSurveillance:getAtHM()
	SelLog.log('I', "La surveillance de température se termine à ".. hc ..":".. mc)
end

notifRecalcul:Launch()
