-- Generic class to handle MQTT stuffs

function MQTTdata( aname, atpc )
	local self = {}

	-- Private fields
	local name = aname
	local topic = atpc

	-- methods
	function self.getName()
		return name
	end

	function self.getTopic()
		return topic
	end

	return self
end
