-- Mise à jour du timer de la consigne "ConsigneCoucher"
--
-->> listen=ConsigneCoucher
-->> waitfor=CoucherSoleilChange

if not SelShared.Get('ConsigneCoucher') then	-- appel de la tache avant reception de la consigne
	return
end

local timer = MajordomeTimer.find("ConsigneCoucher", true)
local timerMy = MajordomeTimer.find("ConsigneCoucherMy", true)

local couchersoleil = MajordomeTimer.find("ConsigneCoucherSoleil", true)
local hs,ms = couchersoleil:getAtHM()

local h,m = string.match(string.gsub( SelShared.Get('ConsigneCoucher'), '%.', ':'), "(%d+):(%d+)")

if hs*100+ms < h*100+m then	-- Le soleil se couche avant
	timer:setAtHM( hs, ms )
	h,m = timer:getAtHM()
	SelLog.log('I', "Fermeture des volets des chambres directement à ".. h ..":".. m)

	timerMy:Disable()
else	-- On utilise la consigne
	timer:setAtHM( h, m )
	timerMy:setAtHM( h, m-5 )
	timerMy:Enable()

	h,m = timerMy:getAtHM()
	SelLog.log('I', "Consigne mi-fermeture pour coucher à ".. h ..":".. m)

	h,m = timer:getAtHM()
	SelLog.log('I', "Consigne de coucher à ".. h ..":".. m)
end
