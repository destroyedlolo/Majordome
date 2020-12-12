-- Calcul du mode réel pour la chambre d'amis
-->> listen=Mode
-->> listen=ModeForceChAmis

if not SelShared.Get("Mode") then	-- Lancement avant que le mode soit déterminé
	return
end

local ModeChAmis = MajordomeMQTTTopic.find("ModeChAmis", true)
local res = SelShared.Get("ModeForceChAmis") or "Auto"

if res == "Auto" then	-- La chambre d'amis répond aux règles générales
	res = SelShared.Get("Mode")
	if res == "Vacances" then	-- repasse en "Travail" car il n'y a personne dans la chambre d'amis
		res = "Travail"
	end
end

ModeChAmis:Publish(res)
SelLog.log('M', "Le mode de la chambre d'amis est : " .. res)
