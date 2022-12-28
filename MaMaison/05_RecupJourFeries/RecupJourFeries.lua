-- Récupération de la liste des jours fériés
--
-->> when=0215OrOver

require("socket")
local https = require("ssl.https")

-- détermination de l'année a interrogée
-- Comme le dernier jour férié de l'année est Noël, on avance
-- d'1 semaine histoire de récupéré aussi le prochain jour férié

local t = os.time() + 86400 *7 -- 24 * 60 * 60

local body, code, headers, status = https.request("https://calendrier.api.gouv.fr/jours-feries/metropole/".. os.date('*t', t)['year'] ..".json")


if code == 200 then
	SelLog.log('I', "Recuperation des jours feries ".. status)
else
	SelLog.log('E', "Recuperation des jours feries ".. status)
end


-- Publie dans le topic
local JourFeries = MajordomeMQTTTopic.find("JourFeries", true)
JourFeries:Publish(body)

