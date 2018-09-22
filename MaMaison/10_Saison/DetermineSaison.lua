-->> waitfor=22h

SelLog.log('M',"Détermination de la saison")

local Saison = MajordomeMQTTTopic.find("Saison")
assert(Saison, "'Saison' pas trouvé")
local SaisonHier = MajordomeMQTTTopic.find("SaisonHier")
assert(SaisonHier, "'SaisonHier' pas trouvé")


SelLog.log('M',"T°Ext = " .. SelShared.Get('TExterieur') .. " TSalon = " .. (SelShared.Get('TSalon') or '??') )
