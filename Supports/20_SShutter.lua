-- Class to handle shutters

function SShutter(aname, atpc)
	local self = MQTTdata(aname, atpc)

	-- methods
	local function command( cmd )
		if Mode.get() == 'Manuel' then
			SelLog.log("Mode Manuel : Fermeture '".. cmd .."' de '".. self.getName() .."' annulée")
		else
			SelLog.log("Fermeture '".. cmd .."' de '".. self.getName() .."'")
			Brk:Publish(self.getTopic(), cmd)
		end
	end

	function self.My()
		command('My')
	end

	function self.Up()
		command('Up')
	end

	function self.My()
		command('Down')
	end

	return self
end
