-- Calcul des automatismes du volet de la chambre d'amis
--
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChAmis
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=ConsigneMeteoCoucherSoleil
--
-->> require_topic=ModeChAmis
-->> require_topic=LeverSoleil
-->> require_topic=ConsigneMeteoCoucherSoleil
-->> require_topic=Saison
-->> need_timer=OuvertureVoletChAmis
-->> need_timer=FermetureVoletChAmis

--
-- Mise à jour des timers d'ouverture et de fermeture en fonction
-- du soleil et de la saison.
--

local hl,ml = string.match(string.gsub( LeverSoleil:getVal(), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( ConsigneMeteoCoucherSoleil:getVal(), '%.', ':'), "(%d+):(%d+)")

if Saison:getVal() == 'Hiver' then	-- économisons la chaleur
	OuvertureVoletChAmis:setAtHM( hl+1, ml )
	hl,ml = OuvertureVoletChAmis:getAtHM()
	
	FermetureVoletChAmis:setAtHM( hc-2, mc )
	hc,mc = FermetureVoletChAmis:getAtHM()
else -- Autre saison
	OuvertureVoletChAmis:setAtHM( hl, ml )
	FermetureVoletChAmis:setAtHM( hc, mc )
end



