-- Generic class to handle timers

function STimer()
	local self = {}

	-- Private fileds
	local tasks = {}	-- events to be scheduled
	local timer,err

	-- methods
	local function SecureIdx( t )
		return tonumber( string.format("%.2f",t) )
	end

	function self.getTimer()
		return timer
	end

		-- Tasks management
	function self.TaskRemove( func )
		for i,f in pairs(tasks) do
			for k,v in ipairs(f) do
				if v == func then
					table.remove(f, k)
				end
			end
			if #f == 0 then
				tasks[i] = nil	
			end
		end

		SelShared.PushTask( self.Rethink, SelShared.TaskOnceConst("LAST"))
	end

	function self.TaskAdd( when, func )
		when = SecureIdx(when)

		if not func then
			SelLog.log("*E* STimer.TaskAdd( NULL )")
			return
		end

		self.TaskRemove( func )	-- Remove potential previous occurence

		if not tasks[ when ] then
			tasks[ when ] = { func }
		else
			table.insert( tasks[ when ], func )
		end

		SelShared.PushTask( self.Rethink, SelShared.TaskOnceConst("LAST"))
	end
	
		-- Determine next candidate
	function self.Next()
		local dt = os.date("*t")
		local cur = SecureIdx(dt.hour + dt.min/100)
		local k = {}

		for t in pairs(tasks) do table.insert(k, t) end
		table.sort(k)

		for _,t in ipairs(k) do
			if t > cur then
				return t	-- Next one is still today
			end
		end

		return k[1]	-- Next one is the 1st of tomorrow
	end

		-- update timer with next candidate
	function self.Rethink()
		timer:Set { at=self.Next() }
	end

		-- debug
	function self.dump()
		print(universal_tostring(tasks))
	end

		-- Current time
	function self.Current()
		local dt = os.date("*t")
		return dt.hour + dt.min/100
	end

		-- which time it will be in x hh.mm
	function self.WhichTimeIn(x)
		return DEC2DMS(DMS2DEC( self.Current() ) + DMS2DEC(x))
	end
	
	function CronExec()
		local dt = os.date("*t")
		local cur = SecureIdx(dt.hour + dt.min/100)

		SubTasks( tasks[cur] )

		self.Rethink()
	end
		
	timer, err = SelTimer.create { ifunc=CronExec }
	if err then
		err = "Cron's timer creation error :" .. err
		SelLog.log(err)
		return nil
	end

	return self
end


