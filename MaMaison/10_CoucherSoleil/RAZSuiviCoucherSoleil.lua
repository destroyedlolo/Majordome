-- Le soleil est couché, on remet à zéro le mécanisme de suivi
-->> when=CoucherSoleil

SelLog.log('I', "Le soleil est couché")

-- On commence à suivre le topic de production solaire
local timer = MajordomeTimer.find("CoucherSoleil", true)
timer:Disable()

local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
prodSolaire:Disable()

local timer = MajordomeTimer.find("ConsigneCoucherSoleil", true)
local h,m = timer:getAtHM()

local trace = MajordomeMQTTTopic.find("TraceSuiviCoucherSoleil", true)
trace:Publish("F;" .. os.date("%H:%M" ..";".. h ..':'..m), true)

