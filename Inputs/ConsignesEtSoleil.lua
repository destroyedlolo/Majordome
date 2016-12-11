-- Consigns

local function IConsigns(aname, atpc, adefault)
	local self = MQTTinputs( aname, atpc, TopicDate2Number )

	-- methods
	function self.log()
		SelLog.log( self.getName() ..' : '..  self.get() )
	end

	-- initialiser
	self.set( adefault )
	self.TasksAdd( self.log )	-- log incoming value

	return self
end

THLeve = IConsigns('Heure levé', 'Majordome/HLever', 8.00 )
THCouche = IConsigns('Heure couché', 'Majordome/HCoucher', 21.30 )

-- Sun management


