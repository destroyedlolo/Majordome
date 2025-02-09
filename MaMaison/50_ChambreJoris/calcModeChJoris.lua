-- Calcul du mode réel pour la chambre de Joris
-->> listen=Mode
-->> need_topic=Mode
-->> listen=ModeForceEnfants
-->> need_topic=ModeForceEnfants
-->> listen=ModeForceJoris
-->> need_topic=ModeForceJoris
-->> need_topic=ModeChJoris

local res = SelSharedVar.Get("ModeForceJoris") or "Auto"

if res == "Auto" then
	res = ModeForceEnfants:getVal()
	if res == "Auto" then
		res = Mode:getVal()
	end
end

ModeChJoris:Publish(res)
SelLog.Log('M', "Le mode de la chambre de Joris est : " .. res)
