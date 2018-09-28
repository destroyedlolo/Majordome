-- Suivit de la production solaire jusqu'à son coucher effectif.
-->> listen=TeleInfoProd

if SelShared.Get("TeleInfoProd") > 0 then
	local timer = MajordomeTimer.find("CoucherSoleil")
	assert( timer, '"CoucherSoleil" pas trouvé' )
	timer:Reset()
	SelLog.log('I', "Le soleil n'est pas encore couché")
end
