-- Mise à jour du timer de la consigne "ConsigneCoucherSoleil" en fonction
-- des infos reçues de la météo.
--
-->> listen=CoucherSoleil

local timer = MajordomeTimer.find("ConsigneCoucherSoleil", true)

local h,m = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

timer:setAtHM( h, m )

SelLog.log('I', "Le soleil se couche à ".. h ..":".. m)
