-- Détermine le mode du jour : "Vacances" ou "Travail"
--
-->> listen=JourFeries
-->> need_topic=JourFerieNom
-->> need_topic=ModeAujourdhui
-->> need_topic=JourFerieSuivant
-->> need_topic=JourFerieSuivantNom

local mode
local json = require("dkjson")
local listJF = json.decode(SelSharedVar.Get("JourFeries"))


-- date du jour

local t = os.date('*t')
local dt = string.format("%04d-%02d-%02d", t.year, t.month, t.day)

if listJF[dt] then	-- c'est un jour férié
	mode = 'Vacances'
	JourFerieNom:Publish(listJF[dt], true)
else
	JourFerieNom:Publish('', true)
end

if t['wday'] == 1 or t['wday'] == 7 then
	mode = 'Vacances'
elseif not mode then	-- Par défaut, on bosse
	mode = 'Travail'
end


ModeAujourdhui:Publish(mode, true)

-- Quel est le jour férié suivant
local dtsuiv = "9999-99-99"
for d, n in pairs(listJF) do
	if d > dt and d < dtsuiv then
		dtsuiv = d
	end
end

JourFerieSuivant:Publish(dtsuiv, true)
JourFerieSuivantNom:Publish(listJF[dtsuiv], true)


-- trace result
SelLog.Log('I', "jour : ".. dt .." -> ".. mode)
SelLog.Log('I', "Jour Férié suivant : ".. dtsuiv .." -> ".. listJF[dtsuiv])
