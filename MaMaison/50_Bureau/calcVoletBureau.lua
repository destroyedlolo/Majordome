-- Détermine les heures d'ouverture et de fermeture du bureau
-->> listen=Mode
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=ConsigneMeteoCoucherSoleil
--
-->> require_topic=LeverSoleil
-->> require_topic=ConsigneMeteoCoucherSoleil
-->> require_topic=Saison
--
-->> need_timer=OuvertureVoletBureau
-->> need_timer=FermetureVoletBureau
-->> need_task=FermetureVoletBureauAction

--
-- Récupération des paramètres solaire
--

local hl,ml = string.match(string.gsub( LeverSoleil:getVal(), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( ConsigneMeteoCoucherSoleil:getVal(), '%.', ':'), "(%d+):(%d+)")

--
-- C'est parti
--

OuvertureVoletBureau:Enable()
FermetureVoletBureauAction:Enable()

if Saison:getVal() == 'Hiver' then
	OuvertureVoletBureau:setAtHM( hl, ml + 30 )
	hl,ml = OuvertureVoletBureau:getAtHM()
	SelLog.Log('I', "Le volet du bureau s'ouvrira à ".. hl ..":".. ml)
	
	FermetureVoletBureau:setAtHM( hc, mc - 15 )
	hc,mc = FermetureVoletBureau:getAtHM()
	FermetureVoletBureau:Enable()	-- Fermeture depuis le timer
	SelLog.Log('I', "Le volet du bureau se fermera à ".. hc ..":".. mc)
else -- Autre saison
	OuvertureVoletBureau:setAtHM( hl, ml )
	SelLog.Log('I', "Le volet du bureau s'ouvrira avec le soleil à ".. hl ..":".. ml)

	FermetureVoletBureau:Disable()	-- Fermeture avec le soleil
	SelLog.Log('I', "Le volet du bureau se fermera avec le soleil à ".. hc ..":".. mc)
end
