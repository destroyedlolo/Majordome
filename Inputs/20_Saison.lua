-- Seasons
local function ASaison()
	self = MQTTinputDefLog('Saison', 'Majordome/Saison', nil, 'Intersaison')

	-- Methods

	function self.DetermineSaison()
		SelLog.log("Détermination de la saison")

		SelLog.log("T°Ext = " .. TExterieur.get() .. " TSalon = " .. TSalon.get())
		Brk:Publish( SaisonHier.getTopic(), Saison.get(), true )

		if TExterieur.get() < 5 then
			Brk:Publish( Saison.getTopic(), 'Hiver', true )
		elseif TSalon.get() > 21.5 and TExterieur.get() > 15 then
			Brk:Publish( Saison.getTopic(), 'Ete', true )
		else
			Brk:Publish( Saison.getTopic(), 'Intersaison', true )
		end
	end
	Timer.TaskAdd( 22.00, self.DetermineSaison )	-- Season is calculated everyday at 22:00

	return self
end

Saison = ASaison()
SaisonHier = MQTTinputDefLog('Saison Hier', 'Majordome/Saison/Hier', nil, 'Intersaison')
