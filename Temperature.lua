-- This script handles stuffs related to temperatures 

-- "Temperature Exterieure"

TExt_tasks = {}	-- Tasks to launch when a TExt is received

function hTExt()	-- Handle tasks at T°Ext arrival
	SubTasks( TExt_tasks )
end

table.insert( Topics, { topic=TExterieur, func=Topic2Number, trigger=hTExt, trigger_once=true } )

-- "Temperature Salon"

TSalon_tasks = {}	-- Tasks to launch when a TExt is received

function hTSalon()	-- Handle tasks at T°Ext arrival
	SubTasks( TSalon_tasks )
end

table.insert( Topics, { topic=TSalon, func=Topic2Number, trigger=hTSalon, trigger_once=true } )

