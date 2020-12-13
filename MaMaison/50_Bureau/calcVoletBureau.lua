-- Détermine les heures d'ouverture et de fermeture du bureau
-->> listen=Mode
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=ConsigneMeteoCoucherSoleil

--
-- Vérification que tous les paramètres sont bien présents
--

if not SelShared.Get("LeverSoleil") 
or not SelShared.Get("ConsigneMeteoCoucherSoleil")
or not SelShared.Get("Saison") then
	return
end

--
-- Récupération des objets cibles
--

local OuvertureVoletBureau = MajordomeTimer.find("OuvertureVoletBureau", true)
local FermetureVoletBureau = MajordomeTimer.find("FermetureVoletBureau", true)
local FermetureVoletBureauAction = MajordomeTask.find("FermetureVoletBureau", true)

--
-- Récupération des paramètres solaire
--

local hl,ml = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( SelShared.Get('ConsigneMeteoCoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

--
-- C'est parti
--

OuvertureVoletBureau:Enable()
FermetureVoletBureauAction:Enable()

if SelShared.Get("Saison") == 'Hiver' then
	OuvertureVoletBureau:setAtHM( hl, ml + 30 )
	hl,ml = OuvertureVoletBureau:getAtHM()
	SelLog.log('I', "Le volet du bureau s'ouvrira à ".. hl ..":".. ml)
	
	FermetureVoletBureau:setAtHM( hc, mc - 15 )
	hc,mc = FermetureVoletBureau:getAtHM()
	FermetureVoletBureau:Enable()	-- Fermeture depuis le timer
	SelLog.log('I', "Le volet du bureau se fermera à ".. hc ..":".. mc)
else -- Autre saison
	OuvertureVoletBureau:setAtHM( hl, ml )
	SelLog.log('I', "Le volet du bureau s'ouvrira avec le soleil à ".. hl ..":".. ml)

	FermetureVoletBureau:Disable()	-- Fermeture avec le soleil
	SelLog.log('I', "Le volet du bureau se fermera avec le soleil à ".. hc ..":".. mc)
end
