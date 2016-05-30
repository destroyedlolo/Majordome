-- Determine activity mode

--
-- Determine the mode to use
--
function updateMode()
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

--
-- The mode changed
--

Mode_tasks = { }

function hModeChanged()
	SubTasks( Mode_tasks )
	SelLog.log("Le mode est maintenant : " .. SelShared.get(MODE))
end

table.insert( Topics, { topic = MODE, trigger=hModeChanged, trigger_once=true } )
