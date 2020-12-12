-- Calcul des automatismes du volet de la chambre d'amis
--
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChAmis
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=ConsigneMeteoCoucherSoleil

-- Vérification que tous les paramètres sont déjà présents
if not SelShared.Get("ModeChAmis")
or not SelShared.Get("LeverSoleil") 
or not SelShared.Get("ConsigneMeteoCoucherSoleil")
or not SelShared.Get("Saison") then
	return
end


--
-- Récupération des timers cibles
--

local OuvertureVoletChAmis = MajordomeTimer.find("OuvertureVoletChAmis", true)
local FermetureVoletChAmis = MajordomeTimer.find("FermetureVoletChAmis", true)


--
-- Mise à jour des timers d'ouverture et de fermeture en fonction
-- du soleil et de la saison.
--

local hl,ml = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( SelShared.Get('ConsigneMeteoCoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Saison") == 'Hiver' then	-- économisons la chaleur
	OuvertureVoletChAmis:setAtHM( hl+1, ml )
	hl,ml = OuvertureVoletChAmis:getAtHM()
	
	FermetureVoletChAmis:setAtHM( hc-2, mc )
	hc,mc = FermetureVoletChAmis:getAtHM()
else -- Autre saison
	OuvertureVoletChAmis:setAtHM( hl, ml )
	FermetureVoletChAmis:setAtHM( hc, mc )
end



