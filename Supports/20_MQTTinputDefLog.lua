-- MQTT data arrival + log when changed + default value

function MQTTinputDefLog(aname, atpc, afunc, adefault, apubdefault )
	local self = MQTTinput( aname, atpc, afunc )

	-- methods
	function self.log()
		SelLog.log( '*>* "' .. self.getName() ..'" : '..  self.get() )
	end

	function self.pubdefault()	-- publish default values
		Brk:Publish( atpc, adefault, true )
	end

	-- initialiser
	self.set( adefault )
	self.TaskAdd( self.log )	-- log incoming value

	if adefault and apubdefault then
		table.insert( lstPubDefault, self )
	end

	return self
end

