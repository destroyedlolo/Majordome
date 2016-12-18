-- Sun always shines on TV

local function ISunRise( aname, atpc, afunc, adefault, atimer )
	local self = MQTTInputEven( aname, atpc, afunc, adefault, atimer )

	-- methods
	function log()
		SelLog.log("*I* Le soleil se leve")
	end
	self.EvenTaskAdd( log )

	function updTime()
		self.getTimer().TaskAdd( self.get(), self.EventTaskSubmit )
	end
	self.TaskAdd( updTime )

	return self
end

SunRise = ISunRise('Levé Soleil', TOPIC_METEO .. '/sunrise', TopicDate2Number, 7.00, Timer )
