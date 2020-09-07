-- Détermine le tarif de l'électricité
-- Publication ne se fait que lorsque l'information change
-->> listen=TarifTeleInfo

local TarifTeleInfo = MajordomeMQTTTopic.find("TarifTeleInfo", true)
local TarifElectricite = MajordomeMQTTTopic.find("TarifElectricite", true)

if not SelShared.Get("TarifElectricite") then	-- Pas encore renseigné
	TarifElectricite:Publish( TarifTeleInfo:getVal() )
elseif TarifElectricite:getVal() ~= TarifTeleInfo:getVal() then	-- A changé
	TarifElectricite:Publish( TarifTeleInfo:getVal() )
end
