-- Calcul des automatismes d'ouverture du volet de la chambre des parents
-- La fermeture est gérée directement par ConsigneCoucher
--
-- Paramètres qui vont lancer un re-calcul
-->> listen=ModeChParents
-->> listen=ConsigneLever

-- Vérification que tous les paramètres sont déjà présents
if not SelShared.Get("ModeChParents")
or not SelShared.Get("ConsigneLever") then
	return
end

--
-- Récupération des timers cibles
--

local OuvertureVoletChParents = MajordomeTimer.find("OuvertureVoletChParents", true)
local OuvertureVoletChParentsMy = MajordomeTimer.find("OuvertureVoletChParentsMy", true)
local VoletChParents = MajordomeMQTTTopic.find("CmdVoletChParents", true)

local hl,ml = string.match(string.gsub( SelShared.Get('ConsigneLever'), '%.', ':'), "(%d+):(%d+)")


--
-- Détermination en fonction du mode
--

if SelShared.Get("ModeChParents") == 'Absent' then
	if not SelShared.Get("ChParentsAbsent") then	-- N'envoie l'ordre de fermeture que si ca n'a pas été déjà fait
		VoletChParents:Publish("Down")
		SelLog.log('I', "Le volet de la chambre des parents se ferme")
		SelShared.Set("ChParentsAbsent", 1)
		VoletChParents:Disable()	-- Plus aucun ordre car les volets doivent restés baissés
	end
	SelLog.log('I', "Le volet de la chambre des parents restera fermé")
	return
else
	VoletChParents:Enable()	-- Les volets peuvent être manipulés
	SelShared.Set("ChParentsAbsent", {}) -- Nous ne sommes pas en mode "Absent"
end

if SelShared.Get("ModeChParents") == 'Vacances' then
		-- Pas d'ouverture le matin
	OuvertureVoletChParents:Disable()
	OuvertureVoletChParentsMy:Disable()
	SelLog.log('I', "Pas d'ouverture de la chambre des parents le matin")
else	-- Travail (le mode "Manuel est pris en charge par les scripts d'actions)
	OuvertureVoletChParents:Enable()
	OuvertureVoletChParentsMy:Enable()
	SelLog.log('I', "My puis Ouverture de la chambre des parents le matin (".. hl ..":".. ml ..")")
end

