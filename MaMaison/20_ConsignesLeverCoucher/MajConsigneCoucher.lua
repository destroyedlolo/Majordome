-- Mise à jour du timer de la consigne "ConsigneCoucher"
--
-->> listen=ConsigneCoucher
-->> waitfor=CoucherSoleilChange

if not SelShared.Get('ConsigneCoucher') then	-- La consigne n'est pas encore renseignée
	return
end

-- Liens vers les timers de consignes
local timer = MajordomeTimer.find("ConsigneCoucher", true)
local h,m = string.match(string.gsub( SelShared.Get('ConsigneCoucher'), '%.', ':'), "(%d+):(%d+)")
local timerMy = MajordomeTimer.find("ConsigneCoucherMy", true)

-- Heure de coucher d'après la météo
local couchersoleil = MajordomeTimer.find("HeureDebutSurveillanceCoucherSoleil", true)
local hs,ms = couchersoleil:getAtHM()

-- c'est parti
if hs*100+ms < h*100+m then	-- Le soleil se couche avant la consigne
	timer:setAtHM( hs, ms )
	h,m = timer:getAtHM()
	SelLog.log('I', "Fermeture des volets des chambres directement à ".. h ..":".. m)

	timerMy:Disable()
else	-- Le soleil se couche après la consigne
	timer:setAtHM( h, m )
	timerMy:setAtHM( h, m-5 )
	timerMy:Enable()

	h,m = timerMy:getAtHM()
	SelLog.log('I', "Consigne mi-fermeture pour coucher à ".. h ..":".. m)

	h,m = timer:getAtHM()
	SelLog.log('I', "Consigne de coucher à ".. h ..":".. m)
end
