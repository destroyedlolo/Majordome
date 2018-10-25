-- Le soleil est couché, on remet à zéro le mécanisme de suivit
-->> when=CoucherSoleil

SelLog.log('I', "Le soleil est couché")

-- On commence à suivre le topic de production solaire
local timer = MajordomeTimer.find("CoucherSoleil", true)
timer:Disable()

local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
prodSolaire:Disable()

