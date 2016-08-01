-- Manages Cat flap

--
-- Topics
--

local TCHAT='maison/Volet/Cuisine/Chat'

--
-- Actions
--

function MyTrappeChat()
	VoletMy( TCHAT, 'Trappe du chat' )
end

--
-- Triggers
--

function determinePlanningChat()
	SelLog.log("Détermination du planning pour la trappe du chat")

	if SelShared.get( SAISON ) == 'Hiver' then
		SelLog.log("'My' du chat à 21h")
		tmrAddEntry( tbl_timers, 21, MyTrappeChat )
	elseif SelShared.get(MODEDEMAIN) == 'Travail' then
		SelLog.log("'My' du chat à 1h")
		tmrAddEntry( tbl_timers, 1, MyTrappeChat )
	elseif SelShared.get(MODEDEMAIN) == 'Vacances' then
		SelLog.log("'My' du chat à 2h")
		tmrAddEntry( tbl_timers, 2, MyTrappeChat )
	else -- Sinon, c'est du manuel et on ne fait rien
		tmrRemoveEntry(tbl_timers, MyTrappeChat)
	end

	SelShared.PushTask( rethingTimerCron, SelShared.TaskOnceConst("LAST"))
end

table.insert( Tasks['Saison'], determinePlanningChat )
table.insert( Tasks['Mode'], determinePlanningChat )
