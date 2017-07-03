-- Sun always shines on TV

local function ISunRise( aname, atpc, afunc, adefault, atimer )
	local self = MQTTInputEven( aname, atpc, afunc, adefault, atimer )

	-- methods
	function log()
		pubLog("*I* Le soleil se leve")
	end
	self.EvenTaskAdd( log )

	function updTime()
		self.getTimer().TaskAdd( self.get(), self.EventTaskSubmit )
	end
	self.TaskAdd( updTime )

	return self
end

SunRise = ISunRise('Levé Soleil', TOPIC_METEO .. '/sunrise', TopicDate2Number, 7.00, Timer )

--------------------

local function ISunSet( aname, atpc, afunc, adefault, atimer )
	local self = MQTTInputEven( aname, atpc, afunc, adefault, atimer )

	-- methods
	function log()
		pubLog("*I* Le soleil se couche")
	end
	self.EvenTaskAdd( log )

	local function SurveillanceCoucheSoleil()
		self.getTimer().TaskRemove(SurveillanceCoucheSoleil)

		if TProdElectrique.get() == nil then	-- No photovolatic production == no sun
			self.EventTaskSubmit()
		else
			local t = self.getTimer().WhichTimeIn(0.01)
			SelLog.log("Le soleil brille toujours : nouveau test à ".. t)

			self.getTimer().TaskAdd( t, SurveillanceCoucheSoleil )
		end
	end

	function updTime()
		self.getTimer().TaskAdd( self.get(), SurveillanceCoucheSoleil )
	end
	self.TaskAdd( updTime )

	return self
end

SunSet = ISunSet('Couché Soleil', TOPIC_METEO .. '/sunset', TopicDate2Number, 21.00, Timer )
