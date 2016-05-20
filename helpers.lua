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
		table.insert(tbl, func )
	end
end

function ColRemoveFunc( tbl, func )
	if type(func) == 'table' then
		local t = {}
		for k,f in pairs(func) do
			t[f]=true
		end
		for k,f in pairs(tbl) do
			if t[f] ~= nil then
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

--
-- DMS
--

function DMS2DEC( val )
	local i,d = math.modf( val )
	return i + d/.6
end

function DEC2DMS( val )
	local i,d = math.modf( val )
	return i + d*.6
end

function TXT2DMS( val )
	local h,m = val:match("(%d+):(%d+)")
	return tonumber(h) + tonumber(m)/100
end

--
-- Timers
--
-- Notez-bien : table *MUST* be ordered
--

function tmrRemoveEntry( tbl, func )
	for i,f in pairs(tbl) do
		for k,v in ipairs(f) do
			if v == func then
				f[k] = nil
			end
		end
		if #f == 0 then
			tbl[i] = nil	
		end
	end
end

function tmrNextTarget( tbl )
	local dt = os.date("*t")
	local cur = dt.hour + dt.min/100
	local first

	for k,v in pairs(tbl) do
		if not first or k < first then
			first = k
		end

		if k > cur then
			return k
		end
	end

	-- The 1st one is tomorrow
	return first	
end

function tmrSubFunc( tbl )
	local dt = os.date("*t")
	local cur = dt.hour + dt.min/100

	SubTasks( tbl[cur] )
end

function SubTasks( tbl )
	-- submit all tasks listed in tbl table


	for _,t in ipairs( tbl ) do
		SelShared.PushTask( t, SelShared.TaskOnceConst("MULTIPLE") )
	end
end


