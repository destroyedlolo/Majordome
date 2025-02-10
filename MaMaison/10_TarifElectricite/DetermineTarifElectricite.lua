-- Détermine le tarif de l'électricité
-- Publication ne se fait que lorsque l'information change
-->> listen=TarifTeleInfo
-->> need_topic=TarifTeleInfo

local TarifElectricite = MajordomeMQTTTopic.find("TarifElectricite", true)

if not SelSharedVar.Get("TarifElectricite") then	-- Pas encore renseigné
	TarifElectricite:Publish( TarifTeleInfo:getVal() )
elseif TarifElectricite:getVal() ~= TarifTeleInfo:getVal() then	-- A changé
	TarifElectricite:Publish( TarifTeleInfo:getVal() )
end
