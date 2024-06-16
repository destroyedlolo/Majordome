-- Calcul du mode réel pour la chambre de parents
-->> listen=Mode
-->> require_topic=Mode
-->> listen=ModeForceParents
-->> need_topic=ModeForceParents
-->> need_topic=ModeChParents

local res = SelSharedVar.Get("ModeForceParents") or "Auto"

if res == "Auto" then
	res = Mode:getVal()
end

ModeChParents:Publish(res)
SelLog.Log('M', "Le mode de la chambre des parents est : " .. res)
