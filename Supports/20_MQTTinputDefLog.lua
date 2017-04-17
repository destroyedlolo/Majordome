-- MQTT data arrival + log when changed + default value

function MQTTinputDefLog(
	aname, atpc, afunc,	-- Topic
	adefault,			-- default value
	apubdefault			-- true : publish the default value, 'N' publish it a numérical value
)
	local self = MQTTinput( aname, atpc, afunc )

	-- methods
	function self.log()
		SelLog.log( '*>* "' .. self.getName() ..'" : '..  self.get() )
	end

	function self.pubdefault()	-- publish default values
		if apubdefault == 'N' then
			Brk:Publish( atpc, string.format('%.2f', adefault), true )
		else
			Brk:Publish( atpc, adefault, true )
		end
	end

	-- initialiser
	self.set( adefault )
	self.TaskAdd( self.log )	-- log incoming value

	if adefault and apubdefault then
		table.insert( lstPubDefault, self )
	end

	return self
end

