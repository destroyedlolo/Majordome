-- MQTT data arrival logged + default value + tracking evens

function MQTTInputEven(aname, atpc, afunc, adefault, atimer)
	local self = MQTTinputDefLog(aname, atpc, afunc, adefault )

	-- private fields
	local eventtasks = {}
	local timer = atimer	-- Which timer to use for this even

	-- methods
	function self.getTimer()
		return timer
	end

	function self.EvenTaskAdd( func )
		if TableTasksAdd( eventtasks, func) == false then
			SelLog.log("*E* MQTTInputEven.EvenTaskAdd( NULL )")
			return
		end
	end

	function self.EvenTaskRemove( func )
		TableTaskRemove( eventtasks, func )
	end

	function self.EventTaskSubmit()
		SubTasks( eventtasks )
	end

	return self
end
