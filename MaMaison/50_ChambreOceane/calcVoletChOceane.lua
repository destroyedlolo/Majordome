-- Calcul des automatismes du volet de la chambre d'Océane
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChOceane
-->> listen=ConsigneLeverOceane

-- Vérification que tous les paramètres sont déjà présents
if not SelShared.Get("ModeChOceane")
or not SelShared.Get("ConsigneLever") then
	return
end


--
-- Récupération des timers cibles
--

local OuvertureVoletChOceane = MajordomeTimer.find("OuvertureVoletChOceane", true)
local OuvertureVoletChOceaneMy = MajordomeTimer.find("OuvertureVoletChOceaneMy", true)

local hl,ml = string.match(string.gsub( SelShared.Get('ConsigneLever'), '%.', ':'), "(%d+):(%d+)")


--
-- Détermination en fonction du mode
--

if SelShared.Get("ModeChOceane") == 'Vacances' then
		-- Pas d'ouverture le matin
	OuvertureVoletChOceane:Disable()
	OuvertureVoletChOceaneMy:Disable()
	SelLog.log('I', "Pas d'ouverture de la chambre d'Océane le matin")

elseif SelShared.Get("ModeChOceane") == 'Absent' then
		-- Ouverture directe le matin (pas de My)
	OuvertureVoletChOceane:Enable()
	OuvertureVoletChOceaneMy:Disable()
	SelLog.log('I', "Ouverture directe de la chambre d'Océane le matin (".. hl ..":".. ml ..")")

else	-- Travail (le mode "Manuel est pris en charge par les scripts d'actions)
	OuvertureVoletChOceane:Enable()
	OuvertureVoletChOceaneMy:Enable()
	SelLog.log('I', "My puis Ouverture de la chambre d'Océane le matin (".. hl ..":".. ml ..")")

end

