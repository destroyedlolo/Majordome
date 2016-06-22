-- Handles working mode

--
-- Topics
--

MODE='Majordome/Mode'
MODEAUJOURDHUI='Majordome/Mode/AujourdHui'
MODEDEMAIN='Majordome/Mode/Demain'
MODEFORCE='Majordome/Mode/Force'

--
-- Lets go
--

function updateTodayMode()	-- Which mode to use
	if not SelShared.get(MODEAUJOURDHUI) then
		SelLog.log("Détermination du mode annulée : MODEAUJOUD'HUI pas encore recu")
		return
	end

	if SelShared.get(MODEFORCE) == 'Auto' then
		SelLog.log("Mode calculé")
		Brk:Publish(MODE, SelShared.get(MODEAUJOURDHUI))
	elseif SelShared.get(MODEFORCE) == 'Manuel' or SelShared.get(MODEFORCE) == 'Travail' or SelShared.get(MODEFORCE) == 'Vacances' then
		SelLog.log("Mode forcé")
		Brk:Publish(MODE, SelShared.get(MODEFORCE))
		SelShared.set(MODEDEMAIN, SelShared.get(MODEFORCE))	-- don't publish a new value to avoid to erase real data
	else	-- Unrecognized mode
		Brk:Publish(MODE, 'Manuel')
		SelLog.log("Mode forcé '".. SelShared.get(MODEFORCE) .."' non reconnu")
	end
end

table.insert( Topics, { topic = MODEAUJOURDHUI, trigger=updateMode, trigger_once=true } )
table.insert( Topics, { topic = MODEDEMAIN, trigger=updateMode, trigger_once=true } )
table.insert( Topics, { topic = MODEFORCE, trigger=updateMode, trigger_once=true } )


