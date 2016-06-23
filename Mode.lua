-- Handles working mode

--
-- Topics
--

MODE='Majordome/Mode'
MODEAUJOURDHUI='Majordome/Mode/AujourdHui'
MODEDEMAIN='Majordome/Mode/Demain'
MODEFORCE='Majordome/Mode/Force'

--
-- Valeurs par défaut
--

SelShared.set( MODE, 'Manuel' )
SelShared.set( MODEDEMAIN, 'Manuel' )
SelShared.set( MODEFORCE, 'Auto' )

--
-- Changement des modes temporels
--

function majModeAujourdhui()
	if SelShared.get(MODEFORCE) == 'Auto' then
		SelLog.log("Mode calculé")
		Brk:Publish(MODE, SelShared.get(MODEAUJOURDHUI))
	end
end

function majModeDemain()
	if SelShared.get(MODEFORCE) ~= 'Auto' then
		SelLog.log("Mode demain forcé à " .. SelShared.get(MODEFORCE) )
		SelShared.set(MODEDEMAIN, SelShared.get(MODEFORCE))
	end
end

function majModeForce()
	if SelShared.get(MODEFORCE) == 'Auto' then
		SelLog.log("Mode repassé à automatique")
		Brk:Publish(MODE, SelShared.get(MODEAUJOURDHUI) or 'Manuel')
		SelShared.set(MODEDEMAIN, 'Manuel')
	elseif SelShared.get(MODEFORCE) == 'Manuel' or SelShared.get(MODEFORCE) == 'Travail' or SelShared.get(MODEFORCE) == 'Vacances' then
		SelLog.log("Mode forcé")
		Brk:Publish(MODE, SelShared.get(MODEFORCE))
		SelShared.set(MODEDEMAIN, SelShared.get(MODEFORCE))
	else
		SelLog.log("Mode forcé '".. SelShared.get(MODEFORCE) .."' non reconnu")	
		Brk:Publish(MODE, 'Manuel')
		SelShared.set(MODEDEMAIN, 'Manuel')
	end
end

table.insert( Topics, { topic = MODEAUJOURDHUI, trigger=majModeAujourdhui, trigger_once=true } )
table.insert( Topics, { topic = MODEDEMAIN, trigger=majModeDemain, trigger_once=true } )
table.insert( Topics, { topic = MODEFORCE, trigger=majModeForce, trigger_once=true } )

--
-- Changement du mode
--

function chgMode()
	SelLog.log("Le mode est maintenant : " .. SelShared.get(MODE))
	SubTasks( Tasks['Mode'] )
end

table.insert( Topics, { topic = MODE, trigger=chgMode, trigger_once=true } )
