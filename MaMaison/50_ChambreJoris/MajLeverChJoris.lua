-- Mise à jour de la consigne "ConsigneLever" de la chambre de Joris
--
-->> listen=ConsigneLeverJoris
--
-->> need_timer=OuvertureVoletChJoris
-->> need_timer=OuvertureVoletChJorisMy

local h,m = string.match(string.gsub( SelSharedVar.Get('ConsigneLeverJoris'), '%.', ':'), "(%d+):(%d+)")

-- Maj des OuvertureVoletChJoriss
OuvertureVoletChJoris:setAtHM( h, m )
OuvertureVoletChJorisMy:setAtHM( h, m-5 )
OuvertureVoletChJorisMy:Enable()

h,m = OuvertureVoletChJorisMy:getAtHM()
SelLog.Log('I', "Consigne de mi-ouverture au lever de la chambre de Joris à ".. h+m/100)

h,m = OuvertureVoletChJoris:getAtHM()
SelLog.Log('I', "Consigne d'ouverture au lever de la chambre de Joris à ".. h+m/100)
