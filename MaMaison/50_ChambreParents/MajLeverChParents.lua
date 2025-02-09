-- Mise à jour de la consigne "ConsigneLever"
--
-->> listen=ConsigneLever
--
-->> need_timer=OuvertureVoletChParents
-->> need_timer=OuvertureVoletChParentsMy

local h,m = string.match(string.gsub( SelSharedVar.Get('ConsigneLever'), '%.', ':'), "(%d+):(%d+)")

-- Maj des OuvertureVoletChParentss
OuvertureVoletChParents:setAtHM( h, m )
OuvertureVoletChParentsMy:setAtHM( h, m-5 )
OuvertureVoletChParentsMy:Enable()

h,m = OuvertureVoletChParentsMy:getAtHM()
SelLog.Log('I', "Consigne de mi-ouverture au lever de la chambre des parents à ".. h+m/100)

h,m = OuvertureVoletChParents:getAtHM()
SelLog.Log('I', "Consigne d'ouverture au lever de la chambre des parents à ".. h+m/100)
