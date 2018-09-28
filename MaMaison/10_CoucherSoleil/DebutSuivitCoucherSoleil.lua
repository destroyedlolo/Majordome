-- L'heure de coucher du soleil est arrivée
-->> until=ConsigneCoucherSoleil

SelLog.log('I', "Début du suivit de la production solaire")

-- On commence à suivre le topic de production solaire
local timer = MajordomeTimer.find("CoucherSoleil")
assert( timer, '"CoucherSoleil" pas trouvé' )
timer:Enable()

local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd")
assert( prodSolaire, '"TeleInfoProd" pas trouvé' )
prodSolaire:Enable()

