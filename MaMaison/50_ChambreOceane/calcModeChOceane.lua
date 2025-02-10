-- Calcul du mode réel pour la chambre d'Oceane
-->> listen=Mode
-->> require_topic=Mode
-->> listen=ModeForceEnfants
-->> need_topic=ModeForceEnfants
-->> listen=ModeForceOceane
-->> need_topic=ModeForceOceane
-->> need_topic=ModeChOceane

local res = SelSharedVar.Get("ModeForceOceane") or "Auto"

if res == "Auto" then
	res = ModeForceEnfants:getVal()
	if res == "Auto" then
		res = Mode:getVal()
	end
end

ModeChOceane:Publish(res)
SelLog.Log('M', "Le mode de la chambre d'Océane est : " .. res)
