-- Parent's room
ChParent = AChambre( "Chambre Parents", 'maison/Volet/chParents', Timer, TChParents, 8.40, 17.30, 21, Mode, HLeve )
ChParent = AChambre( 
	"Chambre Parents", 			-- Name
	'maison/Volet/chParents', 	-- Topic
	Timer, TChParents, 			-- Timer and temperature probe
	8.40, 17.30, 21,			-- Temperature tracking times and limit
	ModeEnfants, HLeveOceane,	-- Mode and wakeup consign
	false						-- true if shutter goes My before Down
)

