-- Calcul des automatismes du volet de la chambre d'amis
--
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChAmis
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=CoucherSoleil

-- Vérification que tous les paramètres sont déjà présents
if not SelShared.Get("ModeChAmis")
or not SelShared.Get("LeverSoleil") 
or not SelShared.Get("CoucherSoleil")
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
local hc,mc = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Saison") == 'Hiver' then	-- économisons la chaleur
	OuvertureVoletChAmis:setAtHM( hl+1, ml )
	hl,ml = OuvertureVoletChAmis:getAtHM()
	
	FermetureVoletChAmis:setAtHM( hc-2, mc )
	hc,mc = FermetureVoletChAmis:getAtHM()
else -- Autre saison
	OuvertureVoletChAmis:setAtHM( hl, ml )
	FermetureVoletChAmis:setAtHM( hc, mc )
end


--
-- On tient compte du mode
--
if SelShared.Get("ModeChAmis") == 'Absent' or SelShared.Get("ModeChAmis") == 'Manuel' then -- Ces 2 modes désactive les automatismes du volet
	OuvertureVoletChAmis:Disable()
	FermetureVoletChAmis:Disable()

	if SelShared.Get("ModeChAmis") == 'Absent' then
		SelLog.log('I', "Le volet de la chambre d'amis restera fermé")

		if not SelShared.Get("ChAmisAbsent") then	-- N'envoie l'ordre de fermeture que si ca n'a pas été déjà fait
			local VoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
			VoletChAmis:Publish("Down")
			SelLog.log('I', "Le volet de la chambre d'amis se ferme")
			SelShared.Set("ChAmisAbsent", 1)
		end
	else	-- Mode manuel
		SelShared.Set("ChAmisAbsent", {}) -- Nous ne sommes pas en mode "Absent"
		SelLog.log('I', "Le volet de la chambre d'amis n'est plus soumis à aucun automatisme")
	end

elseif SelShared.Get("ModeChAmis") == 'Vacances' then -- La chambre est occupée par des vacanciers
	SelShared.Set("ChAmisAbsent", {}) -- Nous ne sommes pas en mode "Absent"

	OuvertureVoletChAmis:Disable()
	FermetureVoletChAmis:Enable()

	SelLog.log('I', "Le volet de la chambre d'amis ne s'ouvrira pas")
	SelLog.log('I', "Le volet de la chambre d'amis se fermera à ".. hc ..":".. mc)

else -- autre modes
	SelShared.Set("ChAmisAbsent", {}) -- Nous ne sommes pas en mode "Absent"

	OuvertureVoletChAmis:Enable()
	FermetureVoletChAmis:Enable()

	SelLog.log('I', "Le volet de la chambre d'amis s'ouvrira à ".. hl ..":".. ml)
	SelLog.log('I', "Le volet de la chambre d'amis se fermera à ".. hc ..":".. mc)	
end
