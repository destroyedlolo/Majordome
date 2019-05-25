-- Publication des consignes par défaut
--
-->> RunAtStartup

-- Récupération des topics à publier

local ConsigneCoucher = MajordomeMQTTTopic.find("ConsigneCoucher", true)
local ConsigneLever = MajordomeMQTTTopic.find("ConsigneLever", true)
local ConsigneLeverJoris = MajordomeMQTTTopic.find("ConsigneLeverJoris", true)
local ConsigneLeverOceane = MajordomeMQTTTopic.find("ConsigneLeverOceane", true)

-- Publication des consignes par défauts

ConsigneCoucher:Publish("21.30")	-- Fermeture des volets des chambres
ConsigneLever:Publish("08.05")		-- Ouverture des volets des parents
ConsigneLeverJoris:Publish("07.05")		-- Ouverture des volets de Joris
ConsigneLeverOceane:Publish("07.00")		-- Ouverture des volets d'Océane

SelLog.log('I', "Les consignes par défauts ont été publiées")
