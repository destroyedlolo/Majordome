-- Work on Saisons

function DetermineSaison()
	local saison
	local t = tonumber(SelShared.get( TExterieur )) 

	if t < 2 then
		saison = 'Hivers'
	elseif t > 10 then
		saison = 'Ete'
	else
		saison = 'Intersaison'
	end

	SelLog.log("T°Ext = " .. t .. " => " .. saison)
	Brk:Publish( SAISON, saison )
	SelShared.set( SAISON, saison )

	ColRemoveFunc( TExt_tasks, DetermineSaison )	-- Not needed anymore
end

function launchDetermineSaison()
	ColAddFunc( TExt_tasks, DetermineSaison )
end

-- Actions related to season change

Saison_tasks = {}	-- Tasks to launch at Season change

function hSaison()
	SubTasks( Saison_tasks )
end

table.insert( Topics, { topic = SAISON, trigger=hSaison, trigger_once=true } )
