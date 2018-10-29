-- Détermine les heures d'ouverture et de fermeture de la chambre d'amis.
-->> listen=ModeChAmis
-->> listen=Saison
-->> listen=LeverSoleil
-->> listen=CoucherSoleil

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
-- Calcul solaire
--

local hl,ml = string.match(string.gsub( SelShared.Get('LeverSoleil'), '%.', ':'), "(%d+):(%d+)")
local hc,mc = string.match(string.gsub( SelShared.Get('CoucherSoleil'), '%.', ':'), "(%d+):(%d+)")

if SelShared.Get("Saison") == 'Hiver' then
	OuvertureVoletChAmis:setAtHM( hl+1, ml )
	hl,ml = OuvertureVoletChAmis:getAtHM()
	
	FermetureVoletChAmis:setAtHM( hc-2, mc )
	hc,mc = FermetureVoletChAmis:getAtHM()
else -- Autre saison
	OuvertureVoletChAmis:setAtHM( hl, ml )
	FermetureVoletChAmis:setAtHM( hc, mc )
end

--
-- Calcul du mode
--

if SelShared.Get("ModeChAmis") == 'Absent' or SelShared.Get("ModeChAmis") == 'Manuel' then
	OuvertureVoletChAmis:Disable()
	FermetureVoletChAmis:Disable()

	if SelShared.Get("ModeChAmis") == 'Absent' then
		SelLog.log('I', "Le volet de la chambre d'amis restera fermé")
		if not SelShared.Get("ChAmisAbsent") then	-- Evite d'envoyer plusieurs ordres de fermeture
			local VoletChAmis = MajordomeMQTTTopic.find("CmdVoletChAmis", true)
			VoletChAmis:Publish("Down")
			SelLog.log('I', "Le volet de la chambre d'amis se ferme")
			SelShared.Set("ChAmisAbsent", 1)
		end
	else
		SelShared.Set("ChAmisAbsent", {})
		SelLog.log('I', "Le volet de la chambre d'amis ne sera pas commandé")
	end
elseif SelShared.Get("ModeChAmis") == 'Vacances' then
	OuvertureVoletChAmis:Disable()
	FermetureVoletChAmis:Enable()
	
	SelLog.log('I', "Le volet de la chambre d'amis ne s'ouvrira pas")
	SelLog.log('I', "Le volet de la chambre d'amis se fermera à ".. hc ..":".. mc)
	SelShared.Set("ChAmisAbsent", {})
else
	OuvertureVoletChAmis:Enable()
	FermetureVoletChAmis:Enable()
	
	SelLog.log('I', "Le volet de la chambre d'amis s'ouvrira à ".. hl ..":".. ml)
	SelLog.log('I', "Le volet de la chambre d'amis se fermera à ".. hc ..":".. mc)
	SelShared.Set("ChAmisAbsent", {})
	
	SelLog.log('D', "Autre mode pour la chambre d'amis : ".. SelShared.Get("Mode"))
end

