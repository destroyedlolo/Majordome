-- Le soleil est couché, on remet à zéro le mécanisme de suivit
-->> until=CoucherSoleil

SelLog.log('I', "Le soleil est couché")

-- On commence à suivre le topic de production solaire
local timer = MajordomeTimer.find("CoucherSoleil")
assert( timer, '"CoucherSoleil" pas trouvé' )
timer:Disable()

local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd")
assert( prodSolaire, '"TeleInfoProd" pas trouvé' )
prodSolaire:Disable()

