-- Détermine les heures d'ouverture et de fermeture du bureau
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=CoucherSoleil

--
-- Vérification que tous les paramètres sont bien présents
--

if not SelShared.Get("LeverSoleil") 
or not SelShared.Get("CoucherSoleil")
or not SelShared.Get("Saison") then
	return
end

--
-- Récupération des timers cibles
--

local OuvertureVoletBureau = MajordomeTimer.find("OuvertureVoletBureau")
assert(OuvertureVoletBureau, "'OuvertureVoletBureau' pas trouvé")

local FermetureVoletBureau = MajordomeTimer.find("FermetureVoletBureau")
assert(FermetureVoletBureau, "'FermetureVoletBureau' pas trouvé")

local hl,ml = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Saison") == 'Hiver' then
	OuvertureVoletBureau:setAtHM( hl+1, ml )
	hl,ml = OuvertureVoletBureau:getAtHM()
	SelLog.log('I', "Le volet du bureau s'ouvrira à ".. hl ..":".. ml)
	
	FermetureVoletBureau:setAtHM( hc-1, mc )
	hc,mc = FermetureVoletBureau:getAtHM()
	SelLog.log('I', "Le volet du bureau se fermera à ".. hc ..":".. mc)
else -- Autre saison
	OuvertureVoletBureau:setAtHM( hl, ml )
	SelLog.log('I', "Le volet du bureau s'ouvrira avec le soleil à ".. hl ..":".. ml)

	FermetureVoletBureau:setAtHM( hc, mc )
	SelLog.log('I', "Le volet du bureau se fermera avec le soleil à ".. hc ..":".. mc)
end
