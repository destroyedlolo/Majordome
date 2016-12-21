-- DMS

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

-- Topic convertion

function Topic2Number( t, v )
	SelShared.set(t, tonumber(v))
	return true
end

function Topic2NumberTTL( t, v, to )
-- to : ttl de la variable, en seconde
	SelShared.set(t, tonumber(v), to)
	return true
end

function TopicDate2Number( t, v )
	SelShared.set(t, TXT2DMS(v))
	return true
end

-- Tasks submition
function TableTasksAdd(tbl, func )
	if not func then
		return false
	end

	if type(func) == 'table' then
		for _,f in ipairs(func) do
			TableTasksAdd(tbl, f)
		end
	else
		for _,f in ipairs(tbl) do	-- Avoid duplicate submission
			if f == func then
				return true
			end
		end
		table.insert(tbl, func )
	end
	return true
end

function TableTaskRemove( tbl, func )
	for i,v in ipairs( tbl ) do
		if v == func then
			table.remove( tbl, i )
		end
	end
end

function SubTasks( tasks, aonce )
	local once
	if aonce then
		once = SelShared.TaskOnceConst("LAST")
	else
		once = SelShared.TaskOnceConst("MULTIPLE")
	end
	
		
	if type(tasks) ~= 'table' then
		return
	end

	for z,t in ipairs( tasks ) do
		if type(t) == 'table' then
			for i,j in ipairs(t) do
				SelShared.PushTask( j, once )
			end
		else
			SelShared.PushTask( t, once )
		end
	end
end
