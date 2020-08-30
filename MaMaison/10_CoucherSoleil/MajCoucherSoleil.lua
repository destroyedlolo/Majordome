-- Mise à jour du timer de la consigne "ConsigneMeteoCoucherSoleil" en fonction
-- des infos reçues de la météo.
--
-->> listen=ConsigneMeteoCoucherSoleil
-->> listen=Saison

-- Toutes les données sont-elles là
if not SelShared.Get('ConsigneMeteoCoucherSoleil') then
	return
end

local timer = MajordomeTimer.find("HeureDebutSurveillanceCoucherSoleil", true)
local timerFin = MajordomeTimer.find("HeureFinSurveillanceCoucherSoleil", true)

local h,m = string.match(string.gsub( SelShared.Get('ConsigneMeteoCoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

SelLog.log('I', "Le soleil se couche à ".. h ..":".. m)

local saison = SelShared.Get("Saison") or "Hiver"
if saison == "Hiver" then
	timer:setAtHM( h, m-15 )
	h,m = timer:getAtHM()
	SelLog.log('I', "Début de la surveillance solaire à ".. h ..":".. m)
else
	timer:setAtHM( h, m )
end

timerFin:setAtHM( h, m+30 )
h,m = timer:getAtHM()
SelLog.log('I', "Fin de la surveillance solaire à ".. h ..":".. m)

-- Les consignes ont changées
local rdv = MajordomeRendezVous.find("CoucherSoleilChange", true)
rdv:Launch()
