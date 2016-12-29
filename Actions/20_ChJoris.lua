-- Joris's room
ChOceane = AChambre( 
	"Chambre de Joris", 		-- Name
	'maison/Volet/chJoris', 	-- Topic
	Timer, TChJoris, 			-- Timer and temperature probe
	9.00, 17.30, 21,			-- Temperature tracking times and limit
	ModeEnfants, HLeveJoris,	-- Mode and wakeup consign
	true						-- true if shutter goes My before Down
)

