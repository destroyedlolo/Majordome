-- Generic class to handle input coming from MQTT

function MQTTinput(aname, atpc, afunc)
	local self = MQTTdata(aname, atpc)

	-- Private fields
	local tasks = {}

	-- methods
	function self.get()
		return SelShared.get( self.getTopic() )
	end

	function self.set( v )
		SelShared.set( self.getTopic(), v )
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

