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

function SubTasks( tasks )
	if type(tasks) ~= 'table' then
		return
	end

	for z,t in ipairs( tasks ) do
		if type(t) == 'table' then
			for i,j in ipairs(t) do
				SelShared.PushTask( j, SelShared.TaskOnceConst("MULTIPLE") )
			end
		else
			SelShared.PushTask( t, SelShared.TaskOnceConst("MULTIPLE") )
		end
	end
end
