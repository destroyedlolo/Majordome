-- Publication des consignes par défaut
--
-- Ne sera lancée que par la commande "CONSIGNES" arrivant sur le topic de debug
--
-->> need_topic=ConsigneCoucher
-->> need_topic=ConsigneLever
-->> need_topic=ConsigneLeverJoris
-->> need_topic=ConsigneLeverOceane

ConsigneCoucher:Publish("21.30")	-- Fermeture des volets des chambres
ConsigneLever:Publish("08.05")		-- Ouverture des volets des parents
ConsigneLeverJoris:Publish("07.05")		-- Ouverture des volets de Joris
ConsigneLeverOceane:Publish("07.00")		-- Ouverture des volets d'Océane

SelLog.Log('I', "Les consignes par défauts ont été publiées")
