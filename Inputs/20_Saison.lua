-- Seasons

Saison = MQTTinputDefLog('Saison', 'Majordome/Saison', nil, 'Intersaison')
SaisonHier = MQTTinputDefLog('Saison Hier', 'Majordome/Saison/Hier', nil, 'Intersaison')

--
-- Actions
--

local function DetermineSaison()
	SelLog.log("Détermination de la saison")

	SelLog.log("T°Ext = " .. TExterieur.get() .. " TSalon = " .. TSalon.get())

	if TExterieur.get() < 2 then
		Brk:Publish( Saison.getTopic(), 'Hiver', true )
	elseif TSalon.get() > 21.5 and TExterieur.get() > 15 then
		Brk:Publish( Saison.getTopic(), 'Ete', true )
	else
		Brk:Publish( Saison.getTopic(), 'Intersaison', true )
	end
end
Timer.TaskAdd( 22.00, DetermineSaison )	-- Season is calculated at 22:00

local function UpdYesterdaySeason()
	Brk:Publish( SaisonHier.getTopic(), Saison.get(), true )
end
Saison.TaskAdd(UpdYesterdaySeason)
