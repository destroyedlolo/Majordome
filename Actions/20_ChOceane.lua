-- Oceane's room
ChOceane = AChambre( 
	"Chambre d'Océane", 		-- Name
	'maison/Volet/chOceane', 	-- Topic
	Timer, TChOceane, 			-- Timer and temperature probe
	9.00, 17.30, 21,			-- Temperature tracking times and limit
	ModeEnfants, HLeveOceane,	-- Mode and wakeup consign
	true						-- true if shutter goes My before Down
)

