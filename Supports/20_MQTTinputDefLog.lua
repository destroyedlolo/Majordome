-- MQTT data arrival + log when changed + default value

function MQTTinputDefLog(aname, atpc, afunc, adefault )
	local self = MQTTinput( aname, atpc, afunc )

	-- methods
	function self.log()
		SelLog.log( '*>* "' .. self.getName() ..'" : '..  self.get() )
	end

	-- initialiser
	self.set( adefault )
	self.TaskAdd( self.log )	-- log incoming value

	return self
end

