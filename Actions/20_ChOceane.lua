-- Oceane's room
ChOceane = AChambre( 
	"Chambre d'Océane",
	'maison/Volet/chOceane',
	'Majordome/Mode/Oceane',
	Timer, TChOceane,
	9.00, 17.30, 21,
	{ ModeFEnfants, ModeFOceane },	-- ModeForce is not needed as overwriting Mode
	HLeverOceane,				-- wakeup consign
	true
)
