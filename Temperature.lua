-- This script handles stuffs related to temperatures 

-- "Temperature Exterieure"

TExt_tasks = {}	-- Tasks to launch when a TExt is received

function hTExt()	-- Handle tasks at T°Ext arrival
	SubTasks( TExt_tasks )
end

table.insert( Topics, { topic=TExterieur, func=Topic2Number, trigger=hTExt, trigger_once=true } )

-- "Temperature Salon"

TSalon_tasks = {}	-- Tasks to launch when a T Kitchen is received

function hTSalon()	-- Handle tasks at T°kitchen arrival
	SubTasks( TSalon_tasks )
end

table.insert( Topics, { topic=TSalon, func=Topic2Number, trigger=hTSalon, trigger_once=true } )

-- "Temperature Chambre Océane"

TChO_tasks = {}	-- Tasks to launch when a TChOceane is received

function hTChO()	-- Handle tasks at T°ChOceane arrival
	SubTasks( TSalon_tasks )
end

table.insert( Topics, { topic=TChOceane, func=Topic2Number, trigger=hTChO, trigger_once=true } )

