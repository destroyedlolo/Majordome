-- Mise à zéro des compteurs
-->> when=MidnightOrAtLaunch

-- Valeurs des compteurs au début de la journée
SelSharedVar.Set("CONSOCNTHC", nil)
SelSharedVar.Set("CONSOCNTHP", nil)
SelSharedVar.Set("PRODCNT", nil)

SelLog.Log('T', "RAZ des compteurs d'énergie")
