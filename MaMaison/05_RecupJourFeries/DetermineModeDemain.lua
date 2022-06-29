-- Détermine le mode de demain : "Vacances" ou "Travail"
--
-->> listen=JourFeries

local mode
local json = require("dkjson")
local listJF = json.decode(SelShared.Get("JourFeries"))

-- date de demain

local t = os.time() + 86400 -- 24 * 60 * 60
t = os.date('*t', t)
local dt = string.format("%04d-%02d-%02d", t.year, t.month, t.day)

if listJF[dt] then	-- c'est un jour férié
	mode = 'Vacances'
end

if t['wday'] == 1 or t['wday'] == 7 then
	mode = 'Vacances'
elseif not mode then	-- Par défaut, on bosse
	mode = 'Travail'
end

local ModeDemain = MajordomeMQTTTopic.find("ModeDemain", true)
assert(ModeDemain)
ModeDemain:Publish(mode)

-- trace result
SelLog.log('I', "Demain : ".. dt .." -> ".. mode)
