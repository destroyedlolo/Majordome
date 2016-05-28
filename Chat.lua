-- Cat entrance

function MyTrappeChat()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture 'My' de la trappe du chat annulée")
	else
		SelLog.log("Fermeture 'My' de la trappe du chat")

		Brk:Publish( 'maison/Volet/Cuisine/Chat', 'My' )
	end
end

function OuvreTrappeChat()
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture de la trappe du chat annulée")
	else
		SelLog.log("Ouverture de la trappe du chat")

		Brk:Publish( 'maison/Volet/Cuisine/Chat', 'Up' )
	end
end

--
-- triggers
--

function determinePlanningChat()
	SelLog.log("Détermination du planning pour la trappe du chat")
	tmrRemoveEntry(tbl_timers, MyTrappeChat)

	if SelShared.get( SAISON ) == 'Hiver' then
		SelLog.log("'My' du chat à 21h")
		tmrAddEntry( tbl_timers, 21, MyTrappeChat )
	elseif SelShared.get(MODE) == 'Travail' then
		SelLog.log("'My' du chat à 1h")
		tmrAddEntry( tbl_timers, 1, MyTrappeChat )
	else
		SelLog.log("'My' du chat à 2h")
		tmrAddEntry( tbl_timers, 2, MyTrappeChat )
	end
end

table.insert( Mode_tasks, determinePlanningChat )
table.insert( Saison_tasks, determinePlanningChat )

