-- Work on Saisons

function DetermineSaison()
	local saison
	local t = tonumber(SelShared.get( TExterieur )) 

	if t < 5 then
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
