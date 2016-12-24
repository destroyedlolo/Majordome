-- Parent's room
local function AParent ()
	local self = SShutterTempTracking("Chambre Parents", 'maison/Volet/chParents', Timer, TChParents, 8.40, 17.30, 21)

	-- methods
	local function determinePlaning()
		self.getTimer().TaskRemove( self.My )
		self.getTimer().TaskRemove( self.Up )
		self.getTimer().TaskRemove( self.Down )
		SunRise.EvenTaskRemove( self.Up )

		local h
		if Mode.get() == 'Travail' then
			h = self.getTimer().AddTime( HLeve.get(), -0.05 )
			SelLog.log( self.getName() .. ' : Ouverture "My" à ' .. h )
			self.getTimer().TaskAdd( h, self.My )
			SelLog.log( self.getName() .. ' : Ouverture à ' .. HLeve.get() )
			self.getTimer().TaskAdd( HLeve.get(), self.Up )
			self.LaunchTrackingAt()
		elseif Mode.get() == 'Vacances' then
			SelLog.log( self.getName() .. " : Pas d'ouverture, nous sommes en vacances" )
			self.LaunchTrackingAt( 11.3 )
		elseif Mode.get() == 'Absent' then
			SunRise.EvenTaskAdd( self.Up )
			SelLog.log( self.getName() .. " : Ouverture en fonction du soleil" )
			self.LaunchTrackingAt()
		end

		if SunSet.get() > HCouche.get() then -- Sunset before consign
			SunSet.TaskAdd( self.Down )
			SelLog.log( self.getName() .. " : Fermeture en fonction du soleil" )
		else
			SunSet.TaskRemove( self.Down )
			self.getTimer().TaskAdd( HCouche.get(),  self.Down )
			SelLog.log( self.getName() .. " : Fermeture à " .. HCouche.get() )
		end
	end
	Mode.TaskOnceAdd( determinePlaning )
	HLeve.TaskOnceAdd( determinePlaning )
	HCouche.TaskOnceAdd( determinePlaning )
	SunSet.TaskOnceAdd( determinePlaning ) -- Sunrise not needed as opening depends only on the mode

	return self
end
Parent = AParent()
