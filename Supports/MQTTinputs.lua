-- Generic class to handle input coming from MQTT

function MQTTinputs(aname, atpc, afunc)
	local self = {}

	-- Private fields
	local name = aname
	local topic = atpc
	local tasks = {}

	-- methods
	function self.get()
		return SelShared.get( topic )
	end

	function self.set( v )
		SelShared.set( topic, v )
	end

	function self.getName()
		return name
	end

	function self.received()
		self.TasksSubmit()
	end

	function self.TasksAdd( func )
		if not func then
			SelLog.log("*E* MQTTinputs.TasksAdd( NULL )")
			return
		end

		if type(func) == 'table' then
			for _,f in ipairs(func) do
				self.TasksAdd( f )
			end
		else
			for _,f in ipairs( tasks ) do	-- Avoid duplicate submission
				if f == func then
					return
				end
			end
			table.insert(tasks, func )
		end
	end

	function self.TasksSubmit()
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

	-- initialiser
	table.insert( Topics, { topic=atpc, func=afunc, trigger=self.received, trigger_once=true } )

	return self
end

--
-- Helpers
--

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

