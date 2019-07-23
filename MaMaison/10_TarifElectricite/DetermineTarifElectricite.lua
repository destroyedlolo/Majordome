-- Détermine le tarif de l'électricité
-->> listen=TarifTeleInfo

local TarifTeleInfo = MajordomeMQTTTopic.find("TarifTeleInfo", true)
local TarifElectricite = MajordomeMQTTTopic.find("TarifElectricite", true)

if not SelShared.Get("TarifElectricite") then
	TarifElectricite:Publish( TarifTeleInfo:getVal() )
elseif TarifElectricite:getVal() ~= TarifTeleInfo:getVal() then
	TarifElectricite:Publish( TarifTeleInfo:getVal() )
end
