-- Détermine les heures d'ouverture et de fermeture du bureau
-->> listen=Mode
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

local OuvertureVoletBureau = MajordomeTimer.find("OuvertureVoletBureau", true)
local FermetureVoletBureau = MajordomeTimer.find("FermetureVoletBureau", true)

local hl,ml = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Mode") == 'Manuel' then
	OuvertureVoletBureau:Disable()
	FermetureVoletBureau:Disable()

	SelLog.log('I', "Le volet du bureau n'est soumis à aucun automatisme")
else
	OuvertureVoletBureau:Enable()
	FermetureVoletBureau:Enable()

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
end
