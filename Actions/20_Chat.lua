-- Cat flap

function AChat()
	local self = SShutter('Trappe du chat', 'maison/Volet/Cuisine/Chat')

	-- methods
	local function determinePlaning()
		if Saison.get() == 'Hiver' then
			pubLog(self.getName() .. " : 'My' à 21h")
			Timer.TaskAdd( 21.0, self.My )
		elseif ModeDemain.get() == 'Travail' then	-- Eté ou Intersaison
			pubLog(self.getName() .. " : 'My' à 1h")
			Timer.TaskAdd( 1.0, self.My )
		elseif ModeDemain.get() == 'Vacances' then
			pubLog(self.getName() .. " : 'My' à 2h")
			Timer.TaskAdd( 2.0, self.My )
		else
			Timer.TaskRemove( self.My )
		end
	end
	Saison.TaskOnceAdd( determinePlaning )
	Mode.TaskOnceAdd( determinePlaning )
	
	return self
end
Chat = AChat()

