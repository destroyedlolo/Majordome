-- Détermine les heures d'ouverture et de fermeture de la chambre d'amis.
-->> listen=Mode

if not SelShared.Get("Mode") then
	return
end

--
-- Récupération des timers cibles
--

local OuvertureVoletChAmis = MajordomeTimer.find("OuvertureVoletChAmis")
assert(OuvertureVoletChAmis, "'OuvertureVoletChAmis' pas trouvé")

local FermetureVoletChAmis = MajordomeTimer.find("FermetureVoletChAmis")
assert(FermetureVoletChAmis, "'FermetureVoletChAmis' pas trouvé")

if SelShared.Get("Mode") == 'Absent' then
	SelLog.log('I', "Le volet de la chambre d'amis restera fermé")
end
