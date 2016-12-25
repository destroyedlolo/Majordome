-- Kids' room
function AChambre( aname, atpc, atimer, atemp, astart, astop, alimit, amode, ahleve, aMyChouche )
	local self = SShutterTempTracking( aname, atpc, atimer, atemp, astart, astop, alimit )

	-- private fields
	local hleve = ahleve
	local mode = amode
	local couche = aMyChouche

	-- methods
	local function determinePlaning()
		self.getTimer().TaskRemove( self.My )
		self.getTimer().TaskRemove( self.Up )
		self.getTimer().TaskRemove( self.Down )
		SunRise.EvenTaskRemove( self.Up )

		if mode.get() == 'Travail' then
			self.getTimer().AddTime( hleve.get() , -0.1 )
			SelLog.log( self.getName() .. ' : Ouverture "My" à ' .. h )
			self.getTimer().TaskAdd( h, self.My )
			SelLog.log( self.getName() .. ' : Ouverture à ' .. hleve.get() )
			self.getTimer().TaskAdd( hleve.get(), self.Up )
			self.LaunchTrackingAt()
		elseif mode.get() == 'Vacances' then
			SelLog.log( self.getName() .. " : vacances, les volets restent fermés" )
			self.LaunchTrackingAt( 12 )
		elseif mode.get() == 'Absent' then
			SunRise.EvenTaskAdd( self.Up )
			SelLog.log( self.getName() .. " : Ouverture en fonction du soleil" )
			self.LaunchTrackingAt()
		end

		if SunSet.get() < HCouche.get() then -- Sunset before consign
			SunSet.TaskAdd( self.Down )
			SelLog.log( self.getName() .. " : Fermeture en fonction du soleil" )
		else
			local h = self.getTimer().AddTime( HCouche.get(), -.05 )
			SelLog.log( self.getName() .. " : Fermeture 'My' à " .. h )
			SunSet.TaskRemove( self.Down )
			self.getTimer().TaskAdd( HCouche.get(),  self.Down )
			SelLog.log( self.getName() .. " : Fermeture à " .. HCouche.get() )
		end
	end
	mode.TaskOnceAdd( determinePlaning )
	hleve.TaskOnceAdd( determinePlaning )
	HCouche.TaskOnceAdd( determinePlaning )
	SunSet.TaskOnceAdd( determinePlaning ) -- Sunrise not needed as opening depends only on the mode

	return self
end

