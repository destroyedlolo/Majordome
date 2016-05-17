-- This script defiles some helpers

--
-- Collection management
--

function ColAddFunc( tbl, func )
	if type(func) == 'table' then
		for _,f in pairs(func) do
			ColAddFunc( tbl, f )
		end
	else
		tbl:insert( func )
	end
end

function ColRemoveFunc( tbl, func )
	if type(func) == 'table' then
		local t = {}
		for k,f in pairs(func) do
			t[f]=true
		end
		for k,f in pairs(tbl) do
			if t[v] ~= nil then
				tbl[k] = nil
			end
		end
	else
		ColRemoveFunc( tbl, { func } )
	end
end

--
-- Tables
--

-- merge t2 in t1
function TableMerge( t1, t2 )
	for _,v in ipairs(t2) do
		table.insert(t1, v)
	end

	return t1
end

