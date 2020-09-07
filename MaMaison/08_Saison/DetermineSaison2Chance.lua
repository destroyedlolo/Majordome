-- Dans le cas où une température n'est pas encore acquise
-- on attend qu'elle arrive et on relance la détermination de la saison
--
-->> listen=TExterieur
-->> listen=TSalon
-->> once
-->> disabled

-- Cette tâche sera activé si besoin par DetermineSaison

local DetermineSaison = MajordomeTask.find("DetermineSaison", true)
local DetermineSaison2 = MajordomeTask.find("DetermineSaison2Chance", true)

DetermineSaison2:Disable() -- Il a fait son taf, on le redésactive
DetermineSaison:Launch()
