-- L'heure de coucher du soleil est arrivée
-->> when=ConsigneCoucherSoleil

SelLog.log('I', "Début du suivit de la production solaire")

-- On commence à suivre le topic de production solaire
local timer = MajordomeTimer.find("CoucherSoleil", true)
timer:Enable()

local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
prodSolaire:Enable()

