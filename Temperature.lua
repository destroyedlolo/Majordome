-- This script handles stuffs related to "Temperature Exterieur"

TExt_tasks = {}	-- Tasks to launch when a TExt is received

function hTExt()	-- Handle tasks at T°Ext arrival
	SubTasks( TExt_tasks )
end

-- Add related topics

table.insert( Topics, { topic = TExterieur, trigger=hTExt, trigger_once=true } )

