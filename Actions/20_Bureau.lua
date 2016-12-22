-- Dad's office
local function ABureau()
	local self = SShutterTempSurvey('Bureau', 'maison/Volet/Buro', Timer, TBureau, 9.3, 16.3, 21.5)

	-- methods
	 local function determinePlaning()
		SunRise.EvenTaskAdd( self.Up )
		SunSet.EvenTaskAdd( self.Down )
		SelLog.log("Ouverture et fermeture du bureau avec le soleil")

		self.LaunchSurveyAt()
	end
	Mode.TaskOnceAdd( determinePlaning )

	return self
end
Bureau = ABureau()
