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
		if TableTasksAdd( tasks, func ) == false then
			SelLog.log("*E* MQTTinput.TaskAdd( NULL )")
			return
		end
	end

	function self.TaskRemove( func )
		TableTaskRemove( tasks, func )
	end

	function self.TaskSubmit()
		SubTasks( tasks )
	end

	-- initialiser
	table.insert( Topics, { topic=atpc, func=afunc, trigger=self.received, trigger_once=true } )

	return self
end

