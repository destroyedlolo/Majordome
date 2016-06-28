-- This script defiles some helpers

--
-- Actions
--

function VoletMy( topic, nom )
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture 'My' de '".. nom .."' annulée")
	else
		SelLog.log("Fermeture 'My' de '".. nom .."'")
		Brk:Publish(topic, 'My')
	end
end

function VoletOuvre( topic, nom )
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Ouverture de '".. nom .."' annulée")
	else
		SelLog.log("Ouverture de '".. nom .."'")
		Brk:Publish(topic, 'Up')
	end
end

function VoletFerme( topic, nom )
	if SelShared.get(MODE) == "Manuel" then
		SelLog.log("Mode Manuel : Fermeture de '".. nom .."' annulée")
	else
		SelLog.log("Fermeture de '".. nom .."'")
		Brk:Publish(topic, 'Down')
	end
end

--
-- Collection management
--

function ColAddFunc( tbl, func )
	if not func then
		SelLog.log("*D**E* ColAddFunc( NULL )")
		return
	end

	if type(func) == 'table' then
		for _,f in ipairs(func) do
			ColAddFunc( tbl, f )
		end
	else
		for _,f in ipairs( tbl ) do	-- Evite de soumettre plusieurs fois la même fonction
			if f == func then
				return
			end
		end
		table.insert(tbl, func )
	end
end

function ColRemoveFunc( tbl, func )
	for i,v in ipairs( tbl ) do
		if v == func then
			table.remove( tbl, i )
		end
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
	if not func then
		SelLog.log("*D**E* tmrAddEntry( NULL )")
		return
	end

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

	if type(tbl) ~= 'table' then
		return
	end

-- SelLog.log('*d* subtsk type ' .. type(tbl) )
	for z,t in ipairs( tbl ) do
-- SelLog.log('*d* subtsk '.. z ..' type ' .. universal_tostring(t) )
		if type(t) == 'table' then
			for i,j in ipairs(t) do
-- SelLog.log('*d* subtsk' .. i .. ' t:'.. type(j))
				SelShared.PushTask( j, SelShared.TaskOnceConst("MULTIPLE") )
			end
		else
-- SelLog.log('*d* subtsk : une seule fonction t:'.. type(t))
			SelShared.PushTask( t, SelShared.TaskOnceConst("MULTIPLE") )
		end
	end
end

function tmrSubFunc( tbl )
	local dt = os.date("*t")
	local cur = dt.hour + dt.min/100

-- SelLog.log('*d* tmrSubFunc('.. cur ..') ' )
-- SelLog.log('*d* ->' .. universal_tostring(tbl[cur]) )

	SubTasks( tbl[cur] )
end

function tmrRethink( timer, tbl )
	-- update the timer with the next value in its pipe
	-- timer : timer to update
	-- tbl : tasks for this timer

	timer:Set { at=tmrNextTarget(tbl) }
end
