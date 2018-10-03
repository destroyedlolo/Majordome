-- determine les heures d'ouverture et de fermeture du bureau
-->> listen=Saison
-->> until=LeverSoleil
-->> until=CoucherSoleil

local OuvertureVoletBureau = MajordomeTimer.find("OuvertureVoletBureau")
assert(OuvertureVoletBureau, "'OuvertureVoletBureau' pas trouvé")

local FermetureVoletBureau = MajordomeTimer.find("FermetureVoletBureau")
assert(FermetureVoletBureau, "'FermetureVoletBureau' pas trouvé")

if SelShared.Get("Saison") == 'Hiver' then
	
else -- Autre saison
end
