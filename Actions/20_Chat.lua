-- Cat flap

function AChat()
	local self = SShutter('Trappe du chat', 'maison/Volet/Cuisine/Chat')

	-- methods
	local function determinePlaning()
		if Saison.get() == 'Hiver' then
			SelLog.log("'My' du chat � 21h")
			Timer.TaskAdd( 21.0, self.My )
		elseif ModeDemain.get() == 'Travail' then	-- Et� ou Intersaison
			SelLog.log("'My' du chat � 1h")
			Timer.TaskAdd( 1.0, self.My )
		elseif ModeDemain.get() == 'Vacances' then
			SelLog.log("'My' du chat � 2h")
			Timer.TaskAdd( 2.0, self.My )
		else
			Timer.TaskRemove( self.My )
		end
	end
	Saison.TaskAdd( determinePlaning )
	Mode.TaskAdd( determinePlaning )
	
	return self
end
Chat = AChat()

