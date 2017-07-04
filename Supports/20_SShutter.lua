-- Class to handle shutters

function SShutter(aname, atpc)
	local self = MQTTdata(aname, atpc)

	-- methods
	function self.command( cmd, atpc, aname )
		if aname then
			aname = '/'..aname
		else
			aname = ''
		end

		if not atpc then
			atpc = self.getTopic()
		end

		if Mode.get() == 'Manuel' then
			pubLog("Mode Manuel : Fermeture '".. cmd .."' de '".. self.getName() .. aname .."' annulée")
		else
			if cmd == 'Up' then
				pubLog("Ouverture '".. cmd .."' de '".. self.getName() .. aname .. "'", 'Action')
			else
				pubLog("Fermeture '".. cmd .."' de '".. self.getName() .. aname .. "'", 'Action')
			end

			Brk:Publish(atpc, cmd)
		end
	end

	function self.My()
		self.command('My')
	end

	function self.Up()
		self.command('Up')
	end

	function self.Down()
		self.command('Down')
	end

	return self
end
