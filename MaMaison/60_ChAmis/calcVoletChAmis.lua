-- Détermine les heures d'ouverture et de fermeture de la chambre d'amis.
-->> listen=ModeChAmis
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=CoucherSoleil

--
-- Vérification que tous les paramètres sont bien présents
--

if not SelShared.Get("ModeChAmis")
or not SelShared.Get("PrevModeChAmis")
or not SelShared.Get("LeverSoleil") 
or not SelShared.Get("CoucherSoleil")
or not SelShared.Get("Saison") then
	return
end

--
-- Récupération des topics
--

local CmdVoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
local OuvertureVoletChAmis = MajordomeTimer.find("OuvertureVoletChAmis", true)
local FermetureVoletChAmis = MajordomeTimer.find("FermetureVoletChAmis", true)

--
-- C'est parti
--

if SelShared.Get("ModeChAmis") == "Absent" then
	if SelShared.Get("PrevModeChAmis") ~= "Absent" then
		SelLog.log('A', "'Fermeture' forcé du volet de la chambre d'amis")
		CmdVoletChAmis:Publish("Down")
	end
end
