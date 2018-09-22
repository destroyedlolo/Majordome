-- Dans le cas où la température extérieur n'est pas encore acquise
-- on attend qu'elle arrive et on relance la détermination de la saison
--
-->> listen=TExterieur
-->> listen=TSalon
-->> once
-->> disabled

-- Cette tâche sera activé si besoin par DetermineSaison

local DetermineSaison = MajordomeTask.find("DetermineSaison")
assert( DetermineSaison, '"DetermineSaison" pas trouvé')

local DetermineSaison2 = MajordomeTask.find("DetermineSaison2Chance")
assert( DetermineSaison2, '"DetermineSaison2Chance" pas trouvé')
DetermineSaison2:disable() -- Il a fait son taf, on le redésactive

DetermineSaison:Launch()
