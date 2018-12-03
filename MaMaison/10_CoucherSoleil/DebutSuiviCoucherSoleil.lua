-- L'heure de coucher du soleil est arrivée
-->> when=ConsigneCoucherSoleil

SelLog.log('I', "Début du suivi de la production solaire")

-- On commence à suivre le topic de production solaire
local timer = MajordomeTimer.find("CoucherSoleil", true)
timer:Enable()

local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
prodSolaire:Enable()

local trace = MajordomeMQTTTopic.find("TraceSuiviCoucherSoleil", true)
trace:Publish("E;" .. os.date("%H:%M"), true)
