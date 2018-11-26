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
-- Récupération des objets cibles
--

local OuvertureVoletBureau = MajordomeTimer.find("OuvertureVoletBureau", true)
local FermetureVoletBureau = MajordomeTimer.find("FermetureVoletBureau", true)
local FermetureVoletBureauAction = MajordomeTask.find("FermetureVoletBureau", true)

--
-- Récupération des paramètres solaire
--

local hl,ml = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

--
-- C'est parti
--

if SelShared.Get("Mode") == 'Manuel' then
	OuvertureVoletBureau:Disable()
	FermetureVoletBureau:Disable()
	FermetureVoletBureauAction:Disable()

	SelLog.log('I', "Le volet du bureau n'est soumis à aucun automatisme")
end
