-- Kids' room
function AChambre( 
	name, tpc, 			-- Shutter's Topic
	amode,				-- Topic for our own active mode
	timer,				-- Timer to play with
	temp,				-- Temperature probe
	start, stop, limit,	-- Temperature tracking times and limits
	modes,				-- collection of modes that can override automatic calculation
	hlever,				-- Wakeup consign
	MyChoucher			-- true if shutter goes 'My' before Down
)

		-- Initialise temperature tracking
	local self = SShutterTempTracking( name, tpc, timer, temp, start, stop, limit )
	local mymode = MQTTinputDefLog( 'Mode - ' .. name, amode )

	-- methods
	local function determineMode()	-- Determine our own Mode
		local m = Mode.get()	-- By default, we're using the global one
		for _,f in ipairs(modes) do
			if f.get() ~= 'Auto' then
				m = f.get()
				break;
			end
		end
		Brk:Publish( amode, m, true )	-- Publish our new active mode
	end
	Mode.TaskOnceAdd( determineMode )
	for _,f in ipairs(modes) do
		f.TaskOnceAdd( determineMode )
	end

	local function determinePlaning()
		if not mymode.get() then -- Mode not already determined
			return
		end

			-- Clean old settings
		self.getTimer().TaskRemove( self.My )
		self.getTimer().TaskRemove( self.Up )
		self.getTimer().TaskRemove( self.Down )
		SunRise.EvenTaskRemove( self.Up )

			-- Determine planning
		if mymode.get() == 'Travail' then
			local h = self.getTimer().AddTime( hlever.get() , -0.05 )
			pubLog( self.getName() .. ' : Ouverture "My" à ' .. h )
			self.getTimer().TaskAdd( h, self.My )
			pubLog( self.getName() .. ' : Ouverture à ' .. hlever.get() )
			self.getTimer().TaskAdd( hlever.get(), self.Up )
			self.LaunchTrackingAt()
		elseif mymode.get() == 'Vacances' then
			pubLog( self.getName() .. " : vacances, les volets restent fermés" )
			self.LaunchTrackingAt( 12 )
		elseif mymode.get() == 'Absent' then
			SunRise.EvenTaskAdd( self.Up )
			pubLog( self.getName() .. " : Ouverture en fonction du soleil" )
			self.LaunchTrackingAt()
		end

		if mymode.get() ~= 'Manuel' then
			if SunSet.get() < HCoucher.get() then -- Sunset before consign
				SunSet.EvenTaskAdd( self.Down )
				pubLog( self.getName() .. " : Fermeture en fonction du soleil" )
			else
				if MyChoucher then
					local h = self.getTimer().AddTime( HCoucher.get(), -.05 )
					pubLog( self.getName() .. " : Fermeture 'My' à " .. h )
				end
				SunSet.EvenTaskRemove( self.Down )
				self.getTimer().TaskAdd( HCoucher.get(),  self.Down )
				pubLog( self.getName() .. " : Fermeture à " .. HCoucher.get() )
			end
		end
	end
	mymode.TaskOnceAdd( determinePlaning )
	hlever.TaskOnceAdd( determinePlaning )
	HCoucher.TaskOnceAdd( determinePlaning )
	SunSet.TaskOnceAdd( determinePlaning ) -- Sunrise not needed as opening depends only on the mode

	return self
end

