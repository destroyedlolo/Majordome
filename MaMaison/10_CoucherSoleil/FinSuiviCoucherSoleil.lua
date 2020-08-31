-- L'heure de la fin de surveillance du soleil est arrivée
-- (utile dans le cas que la téléinfo est défaillante)
-->> whenStopped=CoucherSoleil
-->> whenDone=CoucherSoleil
--
-- Il n'y a plus de production photo voltaïques

if MAJORDOME_TRACKER_STATUS == 'CHECKING' then -- Dans le cas contraire, les volets avaient déjà été baissés
	local rdv = MajordomeRendezVous.find("CoucherSoleilEffectif", true)
	rdv:Launch()

	-- plus la peine de suivre la production solaire
	local prodSolaire = MajordomeMQTTTopic.find("TeleInfoProd", true)
	prodSolaire:Disable()

	SelLog.log('I', "Fin de la surveillance de la production solaire")
end
