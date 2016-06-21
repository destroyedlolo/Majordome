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
-- MQTT's
--

function Topic2Number( t, v )
	SelShared.set(t, tonumber(v))
	return true
end

function TopicDate2Number( t, v )
	SelShared.set(t, TXT2DMS(v))
	return true
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
	local h,m = string.match(val, "(%d+):(%d+)")
	return tonumber(h) + tonumber(m)/100
end

--
-- Timers
--

function tmrRemoveEntry( tbl, func )
	for i,f in pairs(tbl) do
		for k,v in ipairs(f) do
			if v == func then
				table.remove(f, k)
			end
		end
		if #f == 0 then
			tbl[i] = nil	
		end
	end
end

function tmrAddEntry( tbl, t, func )
	tmrRemoveEntry(tbl, func)
	if not tbl[t] then
		tbl[t] = { func }
	else
		table.insert( tbl[t], func )
	end
end

function _tmrNextTarget( tbl )
	local dt = os.date("*t")
	local cur = dt.hour + dt.min/100
	local k = {}

	for t in pairs(tbl) do table.insert(k, t) end
	table.sort(k)

	for _,t in ipairs(k) do
		if t > cur then
			return t
		end
	end

	-- The 1st one is tomorrow
	return k[1]
end

function tmrNextTarget( tbl )
	local res = _tmrNextTarget( tbl )
SelLog.log('*d* Timer Suivant :' .. res)
	return res
end

function SubTasks( tbl )
	-- submit all tasks listed in tbl table
SelLog.log('*d* subtsk type ' .. type(tbl) )
	for z,t in ipairs( tbl ) do
SelLog.log('*d* subtsk '.. z ..' type ' .. universal_tostring(t) )
		if type(t) == 'table' then
			for i,j in ipairs(t) do
SelLog.log('*d* subtsk' .. i .. ' t:'.. type(j))
				SelShared.PushTask( j, SelShared.TaskOnceConst("MULTIPLE") )
			end
		else
SelLog.log('*d* subtsk : une seule fonction t:'.. type(t))
			SelShared.PushTask( t, SelShared.TaskOnceConst("MULTIPLE") )
		end
	end
end

function tmrSubFunc( tbl )
	local dt = os.date("*t")
	local cur = dt.hour + dt.min/100

SelLog.log('*d* tmrSubFunc('.. cur ..') ' )
SelLog.log('*d* ->' .. universal_tostring(tbl[cur]) )

	SubTasks( tbl[cur] )
end

function tmrRethink( timer, tbl )
	-- update the timer with the next value in its pipe
	-- timer : timer to update
	-- tbl : tasks for this timer

	timer:Set { at=tmrNextTarget(tbl) }
end
