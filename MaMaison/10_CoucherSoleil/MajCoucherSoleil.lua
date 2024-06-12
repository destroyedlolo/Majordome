-- Mise à jour du timer de la consigne "ConsigneMeteoCoucherSoleil" en fonction
-- des infos reçues de la météo.
--
-->> listen=ConsigneMeteoCoucherSoleil
-->> require_topic=ConsigneMeteoCoucherSoleil
-->> listen=Saison
--
-->> need_timer=HeureDebutSurveillanceCoucherSoleil
-->> need_timer=HeureFinSurveillanceCoucherSoleil
--
-->> need_rendezvous=CoucherSoleilChange

local h,m = string.match(string.gsub( SelSharedVar.Get('ConsigneMeteoCoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

SelLog.Log('I', "Le soleil se couche à ".. h ..":".. m)

local saison = SelSharedVar.Get("Saison") or "Hiver"
if saison == "Hiver" then
	HeureDebutSurveillanceCoucherSoleil:setAtHM( h, m-15 )
	h,m = HeureDebutSurveillanceCoucherSoleil:getAtHM()
	SelLog.Log('I', "Début de la surveillance solaire à ".. h ..":".. m)
else
	HeureDebutSurveillanceCoucherSoleil:setAtHM( h, m )
end

HeureFinSurveillanceCoucherSoleil:setAtHM( h, m+30 )
h,m = HeureFinSurveillanceCoucherSoleil:getAtHM()
SelLog.Log('I', "Fin de la surveillance solaire à ".. h ..":".. m)

-- Les consignes ont changées
CoucherSoleilChange:Launch()
