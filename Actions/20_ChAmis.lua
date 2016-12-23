-- Dad's office
local function AChAmis()
	local self = SShutterTempSurvey("Chambre d'Amis", 'maison/Volet/chAmis', Timer, TBureau, 9.0, 16.3, 21.5)

	-- methods
	 local function determinePlaning()
	 	if Mode.get() ~= 'Absent' then
			SunRise.EvenTaskAdd( self.Up )
			SunSet.EvenTaskAdd( self.Down )
			SelLog.log("Ouverture et fermeture de la chambre d'amis avec le soleil")

			self.LaunchSurveyAt()
		end
	end
	Mode.TaskOnceAdd( determinePlaning )

	local function FermeSiAbsent()
	 	if ModeForce.get() == 'Absent' then
			self.Down()
			SunRise.EvenTaskRemove( self.Up )
			SunSet.EvenTaskRemove( self.Down )
		end
	end
	ModeForce.TaskOnceAdd( FermeSiAbsent )

	return self
end
ChAmis = AChAmis()
