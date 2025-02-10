-- Mise à jour du timer de la consigne "ConsigneCoucher"
--
-->> listen=ConsigneCoucher
-->> waitfor=CoucherSoleilChange
--
-->> require_topic=ConsigneCoucher

-- Liens vers les timers de consignes
-- Obligé de faire une recherche par find() vu que le topic et le timer
-- ont le même nom.
local timer = MajordomeTimer.find("ConsigneCoucher", true)
local timerMy = MajordomeTimer.find("ConsigneCoucherMy", true)
local h,m = string.match(string.gsub( ConsigneCoucher:getVal(), '%.', ':'), "(%d+):(%d+)")


-- Heure de coucher d'après la météo
local couchersoleil = MajordomeTimer.find("HeureDebutSurveillanceCoucherSoleil", true)
local hs,ms = couchersoleil:getAtHM()

-- c'est parti
if hs*100+ms < h*100+m then	-- Le soleil se couche avant la consigne
	timer:setAtHM( hs, ms )
	h,m = timer:getAtHM()
	SelLog.Log('I', "Fermeture des volets des chambres directement à ".. h ..":".. m)

	timerMy:Disable()
else	-- Le soleil se couche après la consigne
	timer:setAtHM( h, m )
	timerMy:setAtHM( h, m-5 )
	timerMy:Enable()

	h,m = timerMy:getAtHM()
	SelLog.Log('I', "Consigne mi-fermeture pour coucher à ".. h ..":".. m)

	h,m = timer:getAtHM()
	SelLog.Log('I', "Consigne de coucher à ".. h ..":".. m)
end
