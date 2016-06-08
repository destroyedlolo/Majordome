-- Work on Saisons

function DetermineSaison()
	local saison

	if SelShared.get( TExterieur ) < 2 then
		saison = 'Hivers'
	elseif SelShared.get( TSalon ) > 21 then
		saison = 'Ete'
	else
		saison = 'Intersaison'
	end

	SelLog.log("T°Ext = " .. SelShared.get( TExterieur ) .. " TSalon = " .. SelShared.get( TSalon ) .. " => " .. saison)
	Brk:Publish( SAISON, saison, true )
end

-- Actions related to season change

Saison_tasks = {}	-- Tasks to launch at Season change

function hSaison()
	SubTasks( Saison_tasks )
end

table.insert( Topics, { topic = SAISON, trigger=hSaison, trigger_once=true } )
