-- Mise à jour de la consigne "ConsigneLever" de la chambre d'Océane
--
-->> listen=ConsigneLeverOceane
--
-->> need_timer=OuvertureVoletChOceane
-->> need_timer=OuvertureVoletChOceaneMy

local h,m = string.match(string.gsub( SelSharedVar.Get('ConsigneLeverOceane'), '%.', ':'), "(%d+):(%d+)")

-- Maj des OuvertureVoletChOceanes
OuvertureVoletChOceane:setAtHM( h, m )
OuvertureVoletChOceaneMy:setAtHM( h, m-5 )
OuvertureVoletChOceaneMy:Enable()

h,m = OuvertureVoletChOceaneMy:getAtHM()
SelLog.Log('I', "Consigne de mi-ouverture au lever de la chambre d'Océane à ".. h+m/100)

h,m = OuvertureVoletChOceane:getAtHM()
SelLog.Log('I', "Consigne d'ouverture au lever de la chambre d'Océane à ".. h+m/100)
