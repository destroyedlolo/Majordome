-- Détermine le mode du jour : "Vacances" ou "Travail"
--
-->> listen=JourFeries

local mode
local json = require("dkjson")
local listJF = json.decode(SelShared.Get("JourFeries"))


-- date du jour
local JourFerieNom = MajordomeMQTTTopic.find("JourFerieNom", true)
assert(JourFerieNom)

local t = os.date('*t')
local dt = string.format("%04d-%02d-%02d", t.year, t.month, t.day)

if listJF[dt] then	-- c'est un jour férié
	mode = 'Vacances'
	JourFerieNom:Publish(listJF[dt])
else
	JourFerieNom:Publish('')
end

if t['wday'] == 1 or t['wday'] == 7 then
	mode = 'Vacances'
elseif not mode then	-- Par défaut, on bosse
	mode = 'Travail'
end


local ModeAujourdhui = MajordomeMQTTTopic.find("ModeAujourdhui", true)
assert(ModeAujourdhui)
ModeAujourdhui:Publish(mode)

-- Quel est le jour férié suivant
local dtsuiv = "9999-99-99"
for d, n in pairs(listJF) do
	if d > dt and d < dtsuiv then
		dtsuiv = d
	end
end

local JourFerieSuivant = MajordomeMQTTTopic.find("JourFerieSuivant", true)
assert(JourFerieSuivant)
JourFerieSuivant:Publish(dtsuiv)

local JourFerieSuivantNom = MajordomeMQTTTopic.find("JourFerieSuivantNom", true)
assert(JourFerieSuivantNom)
JourFerieSuivantNom:Publish(listJF[dtsuiv])


-- trace result
SelLog.log('I', "jour : ".. dt .." -> ".. mode)
SelLog.log('I', "Jour Férié suivant : ".. dtsuiv .." -> ".. listJF[dtsuiv])
