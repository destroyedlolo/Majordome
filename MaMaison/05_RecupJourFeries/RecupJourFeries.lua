-- Récupération de la liste des jours fériés
--
-->> when=0215OrOver
-->> need_topic=JourFeries
--
-->> disabled

require("socket")
local https = require("ssl.https")

-- détermination de l'année a interrogée
-- Comme le 31/12 n'est pas férié (malheureusement), on détermine l'année
-- du LENDEMAIN histoire de récupéré aussi le prochain jour férié

local t = os.time() + 86400*7 -- 24 * 60 * 60

local body, code, headers, status = https.request("https://calendrier.api.gouv.fr/jours-feries/metropole/".. os.date('*t', t)['year'] ..".json")


if code == 200 then
	SelLog.Log('I', "Recuperation des jours feries ".. status)
else
	SelLog.Log('E', "Recuperation des jours feries ".. status)
end


-- Publie dans le topic
JourFeries:Publish(body)

