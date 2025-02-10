-- Calcul du mode réel pour la chambre d'amis
-->> listen=Mode
-->> require_topic=Mode
-->> listen=ModeForceChAmis
-->> need_topic=ModeForceChAmis
-->> need_topic=ModeChAmis

local res = ModeForceChAmis:getVal()

if res == "Auto" then	-- La chambre d'amis répond aux règles générales
	res = Mode:getVal()
	if res == "Vacances" then	-- repasse en "Travail" car il n'y a personne dans la chambre d'amis
		res = "Travail"
	end
end

ModeChAmis:Publish(res)
SelLog.Log('M', "Le mode de la chambre d'amis est : " .. res)
