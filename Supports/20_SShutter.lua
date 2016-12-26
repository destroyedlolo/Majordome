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
			SelLog.log("Mode Manuel : Fermeture '".. cmd .."' de '".. self.getName() .. aname .."' annulée")
		else
			if cmd == 'Up' then
				SelLog.log("Ouverture '".. cmd .."' de '".. self.getName() .. aname .. "'")
			else
				SelLog.log("Fermeture '".. cmd .."' de '".. self.getName() .. aname .. "'")
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
