-- L'heure de la fin de surveillance du soleil est arrivée
-- (utile dans le cas que la téléinfo est défaillante)
-->> whenStopped=CoucherSoleil
-->> whenDone=CoucherSoleil
--
-->> need_topic=TeleInfoProd
-->> need_rendezvous=CoucherSoleilEffectif
--
-- Il n'y a plus de production photo voltaïques

if MAJORDOME_TRACKER_STATUS == 'CHECKING' then -- Dans le cas contraire, les volets avaient déjà été baissés
	CoucherSoleilEffectif:Launch()

	-- plus la peine de suivre la production solaire
	TeleInfoProd:Disable()

	SelLog.Log('I', "Fin de la surveillance de la production solaire")
end
