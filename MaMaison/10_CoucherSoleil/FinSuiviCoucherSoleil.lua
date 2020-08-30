-- L'heure de la fin de surveillance du soleil est arrivée
-->> whenStopped=CoucherSoleil

SelLog.log('I', "Fin de la surveillance de la production solaire")

-- On commence à suivre le topic de production solaire
local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
prodSolaire:Disable()

