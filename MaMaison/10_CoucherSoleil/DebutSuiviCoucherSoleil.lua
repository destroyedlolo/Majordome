-- L'heure de coucher du soleil est arrivée
-->> whenStarted=CoucherSoleil

SelLog.log('I', "Début du suivi de la production solaire")

-- On commence à suivre le topic de production solaire
local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
prodSolaire:Enable()

