-- Determine activity mode

--
-- Determine the mode to use
--
function updateMode()
	if SelShared.get(MODEFORCE) == 'Auto' then
		Brk:Publish(MODE, SelShared.get(MODEAUJOURDHUI))
	elseif SelShared.get(MODEFORCE) == 'Manuel' or SelShared.get(MODEFORCE) == 'Travail' or SelShared.get(MODEFORCE) == 'Vacances' then
		Brk:Publish(MODE, SelShared.get(MODEFORCE))
	else	-- Unrecognized mode
		Brk:Publish(MODE, 'Manuel')
		SelLog.log("Mode forcé '".. SelShared.get(MODEFORCE) .."' non reconnu")
	end
end

table.insert( Topics, { topic = MODEAUJOURDHUI, trigger=updateMode, trigger_once=true } )
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
