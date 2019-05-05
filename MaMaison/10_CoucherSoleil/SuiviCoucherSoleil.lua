-- Suivi de la production solaire jusqu'à son coucher effectif.
-->> listen=TeleInfoProd
-->> quiet

if SelShared.Get("TeleInfoProd") > 0 then
	local timer = MajordomeTimer.find("CoucherSoleil", true)
	timer:Reset()
	SelLog.log('I', "Le soleil n'est pas encore couché")
end
