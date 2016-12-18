-- Generic class to handle input coming from MQTT

function MQTTinput(aname, atpc, afunc)
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

	function self.getTopic()
		return topic
	end

	function self.received()
		self.TaskSubmit()
	end

	function self.TaskAdd( func )
		if not func then
			SelLog.log("*E* MQTTinput.TaskAdd( NULL )")
			return
		end

		if type(func) == 'table' then
			for _,f in ipairs(func) do
				self.TaskAdd( f )
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

	function self.TaskRemove( func )
		for i,v in ipairs( tasks ) do
			if v == func then
				table.remove( tasks, i )
			end
		end
	end

	function self.TaskSubmit()
		SubTasks( tasks )
	end

	-- initialiser
	table.insert( Topics, { topic=atpc, func=afunc, trigger=self.received, trigger_once=true } )

	return self
end

