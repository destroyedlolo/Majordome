-- Calcul des automatismes du volet de la chambre d'Océane
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChOceane
-->> listen=ConsigneLeverOceane
-->> require_topic=ModeChOceane
-->> require_topic=ConsigneLeverOceane
-->> need_timer=OuvertureVoletChOceane
-->> need_timer=OuvertureVoletChOceaneMy

local hl,ml = string.match(string.gsub( ConsigneLeverOceane:getVal(), '%.', ':'), "(%d+):(%d+)")


--
-- Détermination en fonction du mode
--

if ModeChOceane:getVal() == 'Vacances' then
		-- Pas d'ouverture le matin
	OuvertureVoletChOceane:Disable()
	OuvertureVoletChOceaneMy:Disable()
	SelLog.Log('I', "Pas d'ouverture de la chambre d'Océane le matin")

elseif ModeChOceane:getVal() == 'Absent' then
		-- Ouverture directe le matin (pas de My)
	OuvertureVoletChOceane:Enable()
	OuvertureVoletChOceaneMy:Disable()
	SelLog.Log('I', "Ouverture directe de la chambre d'Océane le matin (".. hl ..":".. ml ..")")

else	-- Travail (le mode "Manuel est pris en charge par les scripts d'actions)
	OuvertureVoletChOceane:Enable()
	OuvertureVoletChOceaneMy:Enable()
	SelLog.Log('I', "My puis Ouverture de la chambre d'Océane le matin (".. hl ..":".. ml ..")")

end

