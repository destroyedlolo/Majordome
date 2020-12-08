-- Mise à jour de la consigne "ConsigneLever"
--
-->> listen=ConsigneLever

local timer = MajordomeTimer.find("OuvertureVoletChParents", true)
local timerMy = MajordomeTimer.find("OuvertureVoletChParentsMy", true)

local h,m = string.match(string.gsub( SelShared.Get('ConsigneLever'), '%.', ':'), "(%d+):(%d+)")

-- Maj des timers
timer:setAtHM( h, m )
timerMy:setAtHM( h, m-5 )
timerMy:Enable()

h,m = timerMy:getAtHM()
SelLog.log('I', "Consigne de mi-ouverture au lever de la chambre des parents à ".. h+m/100)

h,m = timer:getAtHM()
SelLog.log('I', "Consigne d'ouverture au lever de la chambre des parents à ".. h+m/100)
