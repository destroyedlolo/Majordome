-- Joris's room
ChOceane = AChambre( 
	"Chambre de Joris",
	'maison/Volet/chJoris',
	'Majordome/Mode/Joris',
	Timer, TChJoris,
	9.00, 17.30, 21,			-- Temperature tracking
	{ ModeFEnfants, ModeFJoris },	-- ModeForce is not needed as overwriting Mode
	HLeverJoris,					-- wakeup consign
	true
)

