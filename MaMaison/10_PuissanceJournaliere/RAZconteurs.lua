-- Mise à zéro des compteurs
-->> when=MidnightOrAtLaunch

-- Valeurs des compteurs au début de la journée
SelShared.Set("CONSOCNTHC", nil)
SelShared.Set("CONSOCNTHP", nil)
SelShared.Set("PRODCNT", nil)

SelLog.log('T', "RAZ des compteurs d'énergie")
