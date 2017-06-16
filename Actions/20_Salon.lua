-- Living
function ASalon()
	local self = SShutterTempTracking(
		"Salon", 				-- Name
		'maison/Volet/Salon', 	-- Topic
		Timer, TSalon, 			-- Timer and temperature probe
		10.00, 18.30, 22		-- Temperature tracking times and limit
	)

	-- methods
	function self.My()
		self.command('My', 'maison/Volet/Salon/Balcon', 'Balcon')
		self.command('My', 'maison/Volet/Salon/Fenetre', 'Fenetre')
		self.command('My', 'maison/Volet/Salon/Cheminee', 'Cheminee')
	end

	function self.Up()
		self.command('Up', 'maison/Volet/Salon/Balcon', 'Balcon')
		self.command('Up', 'maison/Volet/Salon/Fenetre', 'Fenetre')
		self.command('Up', 'maison/Volet/Salon/Cheminee', 'Cheminee')
	end

	function self.Down()
		self.command('Down', 'maison/Volet/Salon/Balcon', 'Balcon')
		self.command('Down', 'maison/Volet/Salon/Fenetre', 'Fenetre')
		self.command('Down', 'maison/Volet/Salon/Cheminee', 'Cheminee')
	end

	local function determinePlaning()
			-- Some cleaning
		SunRise.EvenTaskRemove( self.Up )
		SunSet.EvenTaskRemove( self.Down )

			-- Opening
		if Saison.get() == 'Ete' then	-- should stayed My for the night
			SelLog.log( self.getName() .. " : Pas d'ouverture car nous sommes en été")
		else
			SunRise.EvenTaskAdd( self.Up )
			SelLog.log( self.getName() .. " : Ouverture en fonction du soleil" )
		end

			-- Closing
		if Saison.get() == 'Ete' and Mode.get() ~= 'Absent' then
			SunSet.EvenTaskAdd( self.My )
			SelLog.log( self.getName() .. " : Fermeture 'My' en fonction du soleil" )
		else
			SunSet.EvenTaskAdd( self.Down )
			SelLog.log( self.getName() .. " : Fermeture en fonction du soleil" )
		end
		
		self.LaunchTrackingAt()
	end
	Mode.TaskOnceAdd( determinePlaning )
	Saison.TaskOnceAdd( determinePlaning )

	return self
end
Salon = ASalon()

