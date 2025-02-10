-- L'heure de coucher du soleil est arrivée
-->> whenStarted=CoucherSoleil
-->> need_topic=TeleInfoProd

SelLog.Log('I', "Début du suivi de la production solaire")

-- On commence à suivre le topic de production solaire
TeleInfoProd:Enable()

