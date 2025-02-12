-- Calcul des automatismes d'ouverture du volet de la chambre des parents
-- La fermeture est gérée directement par ConsigneCoucher
--
-- Paramètres qui vont lancer un re-calcul
-->> listen=ModeChParents
-->> listen=ConsigneLever
-->> require_topic=ModeChParents
-->> require_topic=ConsigneLever
-->> need_timer=OuvertureVoletChParents
-->> need_timer=OuvertureVoletChParentsMy

--
-- Récupération des timers cibles
--

local VoletChParents = MajordomeMQTTTopic.find("CmdVoletChParents", true)

local hl,ml = string.match(string.gsub( ConsigneLever:getVal(), '%.', ':'), "(%d+):(%d+)")


--
-- Détermination en fonction du mode
--

if ModeChParents:getVal() == 'Absent' then
	if not SelSharedVar.Get("ChParentsAbsent") then	-- N'envoie l'ordre de fermeture que si ca n'a pas été déjà fait
		VoletChParents:Publish("Down")
		SelLog.Log('I', "Le volet de la chambre des parents se ferme")
		SelSharedVar.Set("ChParentsAbsent", 1)
		VoletChParents:Disable()	-- Plus aucun ordre car les volets doivent restés baissés
	end
	SelLog.Log('I', "Le volet de la chambre des parents restera fermé")
	return
else
	VoletChParents:Enable()	-- Les volets peuvent être manipulés
	SelSharedVar.Set("ChParentsAbsent", {}) -- Nous ne sommes pas en mode "Absent"
end

if ModeChParents:getVal() == 'Vacances' then
		-- Pas d'ouverture le matin
	OuvertureVoletChParents:Disable()
	OuvertureVoletChParentsMy:Disable()
	SelLog.Log('I', "Pas d'ouverture de la chambre des parents le matin")
else	-- Travail (le mode "Manuel est pris en charge par les scripts d'actions)
	OuvertureVoletChParents:Enable()
	OuvertureVoletChParentsMy:Enable()
	SelLog.Log('I', "My puis Ouverture de la chambre des parents le matin (".. hl ..":".. ml ..")")
end
