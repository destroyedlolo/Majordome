-- Mise à jour de la consigne "ConsigneLever" de la chambre de Joris
--
-->> listen=ConsigneLeverJoris

local timer = MajordomeTimer.find("OuvertureVoletChJoris", true)
local timerMy = MajordomeTimer.find("OuvertureVoletChJorisMy", true)

local h,m = string.match(string.gsub( SelShared.Get('ConsigneLeverJoris'), '%.', ':'), "(%d+):(%d+)")

-- Maj des timers
timer:setAtHM( h, m )
timerMy:setAtHM( h, m-5 )
timerMy:Enable()

h,m = timerMy:getAtHM()
SelLog.log('I', "Consigne de mi-ouverture au lever de la chambre de Joris à ".. h+m/100)

h,m = timer:getAtHM()
SelLog.log('I', "Consigne d'ouverture au lever de la chambre de Joris à ".. h+m/100)
