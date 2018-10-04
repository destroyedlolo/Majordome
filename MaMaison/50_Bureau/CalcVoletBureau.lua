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

-- print("******* Lancé par trig:", MAJORDOME_TRIGGER, "tmr:", MAJORDOME_TIMER )

if SelShared.Get("Saison") == 'Hiver' then
	local h,m = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
	OuvertureVoletBureau:setAtHM( h+1, m )
	h,m = OuvertureVoletBureau:getAtHM()
	SelLog.log('I', "Le volet du bureau s'ouvrira à ".. h ..":".. m)
	
	h,m = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")
	FermetureVoletBureau:setAtHM( h-1, m )
	h,m = FermetureVoletBureau:getAtHM()
	SelLog.log('I', "Le volet du bureau se fermera à ".. h ..":".. m)
else -- Autre saison
end
