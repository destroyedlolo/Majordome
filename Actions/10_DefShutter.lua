-- default setting for shutters
function DefShutter(aname, atpc, atimer, atemperature, astart, aend, alimit)
	local self = SShutterTempTracking(aname, atpc, atimer, atemperature, astart, aend, alimit)

	-- methods
	 local function determinePlaning()
		SunRise.EvenTaskAdd( self.Up )
		SunSet.EvenTaskAdd( self.Down )
		SelLog.log( self.getName() .. " : Ouverture et fermeture avec le soleil")

		self.LaunchTrackingAt()
	end
	Mode.TaskOnceAdd( determinePlaning )

	return self
end
