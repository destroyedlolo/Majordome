-- Kids' room
function AChambre( aname, atpc, atimer, atemp, astart, astop, alimit, amode, ahleve, aMyChouche )
	local self = SShutterTempTracking( aname, atpc, atimer, atemp, astart, astop, alimit )

	-- private fields
	local hleve = ahleve
	local mode = amode
	local smoothcouche = aMyChouche

	-- methods
	local function determinePlaning()
		self.getTimer().TaskRemove( self.My )
		self.getTimer().TaskRemove( self.Up )
		self.getTimer().TaskRemove( self.Down )
		SunRise.EvenTaskRemove( self.Up )

		local m = mode.get()
		if m == 'Auto' then
			m = Mode.get()
		end

		if m == 'Travail' then
			local h = self.getTimer().AddTime( hleve.get() , -0.05 )
			SelLog.log( self.getName() .. ' : Ouverture "My" à ' .. h )
			self.getTimer().TaskAdd( h, self.My )
			SelLog.log( self.getName() .. ' : Ouverture à ' .. hleve.get() )
			self.getTimer().TaskAdd( hleve.get(), self.Up )
			self.LaunchTrackingAt()
		elseif m == 'Vacances' then
			SelLog.log( self.getName() .. " : vacances, les volets restent fermés" )
			self.LaunchTrackingAt( 12 )
		elseif m == 'Absent' then
			SunRise.EvenTaskAdd( self.Up )
			SelLog.log( self.getName() .. " : Ouverture en fonction du soleil" )
			self.LaunchTrackingAt()
		end

		if SunSet.get() < HCouche.get() then -- Sunset before consign
			SunSet.EvenTaskAdd( self.Down )
			SelLog.log( self.getName() .. " : Fermeture en fonction du soleil" )
		else
			if smoothcouche then
				local h = self.getTimer().AddTime( HCouche.get(), -.05 )
				SelLog.log( self.getName() .. " : Fermeture 'My' à " .. h )
			end
			SunSet.EvenTaskRemove( self.Down )
			self.getTimer().TaskAdd( HCouche.get(),  self.Down )
			SelLog.log( self.getName() .. " : Fermeture à " .. HCouche.get() )
		end
	end
	mode.TaskOnceAdd( determinePlaning )
	if mode ~= Mode then
		Mode.TaskOnceAdd( determinePlaning )
	end
	hleve.TaskOnceAdd( determinePlaning )
	HCouche.TaskOnceAdd( determinePlaning )
	SunSet.TaskOnceAdd( determinePlaning ) -- Sunrise not needed as opening depends only on the mode

	return self
end

