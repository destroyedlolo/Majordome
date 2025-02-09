-- Détermination de l'heure de fin de surveillance en fonction du soleil
-->> listen=ConsigneMeteoCoucherSoleil
-->> listen=Saison
--
-->> require_topic=ConsigneMeteoCoucherSoleil
-->> require_topic=Saison
-->> need_timer=DebutSurveillance
-->> need_timer=FinSurveillance
-->> need_rendezvous=recalculeSurveillance
-->> need_rendezvous=desactiveSurveillance
-->> need_rendezvous=activeSurveillance

--
-- Récupération des infos
--

local hc,mc = string.match(string.gsub( ConsigneMeteoCoucherSoleil:getVal(), '%.', ':'), "(%d+):(%d+)")

if Saison:getVal() == 'Hiver' then	-- L'hiver il n'y a pas de surveillance
	SelLog.Log('I', "C'est l'hiver : Pas de surveillance de la température")

	if DebutSurveillance:isEnabled() then
		DebutSurveillance:Disable()
	end
	if FinSurveillance:isEnabled() then
		FinSurveillance:Disable()
	end

	desactiveSurveillance:Launch()
else	-- Autre saison : on surveille
	if not DebutSurveillance:isEnabled() then
		DebutSurveillance:Enable()
	end
	if not FinSurveillance:isEnabled() then
		FinSurveillance:Enable()
	end

	activeSurveillance:Launch()

	hc = hc - 2;	-- On arrête la surveillance 2 heure avant le coucher du soleil
	if hc < 0 then	-- Evite des valeurs négatives (normalement que pour des tests)
		hc,mc = 0,0
	end

--[[
	if hc > 16 then	-- on force la fin de surveillance à 17:00 si le soleil se couche tard
		hc = 17
		mc = 0
	end
--]]
	FinSurveillance:setAtHM( hc, mc )
	hc,mc = FinSurveillance:getAtHM()
	SelLog.Log('I', "La surveillance de température se termine à ".. hc ..":".. mc)
end

recalculeSurveillance:Launch()
