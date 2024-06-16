-- Calcul des automatismes du volet de la chambre de Joris
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChJoris
-->> listen=ConsigneLeverJoris
-->> require_topic=ModeChJoris
-->> require_topic=ConsigneLeverJoris
-->> need_timer=OuvertureVoletChJoris
-->> need_timer=OuvertureVoletChJorisMy


local hl,ml = string.match(string.gsub( ConsigneLeverJoris:getVal(), '%.', ':'), "(%d+):(%d+)")


--
-- Détermination en fonction du mode
--

if ModeChJoris:getVal() == 'Vacances' then
		-- Pas d'ouverture le matin
	OuvertureVoletChJoris:Disable()
	OuvertureVoletChJorisMy:Disable()
	SelLog.Log('I', "Pas d'ouverture de la chambre de Joris le matin")

elseif ModeChJoris:getVal() == 'Absent' then
		-- Ouverture directe le matin (pas de My)
	OuvertureVoletChJoris:Enable()
	OuvertureVoletChJorisMy:Disable()
	SelLog.Log('I', "Ouverture directe de la chambre d'Joris le matin (".. hl ..":".. ml ..")")

else	-- Travail (le mode "Manuel est pris en charge par les scripts d'actions)
	OuvertureVoletChJoris:Enable()
	OuvertureVoletChJorisMy:Enable()
	SelLog.Log('I', "My puis Ouverture de la chambre de Joris le matin (".. hl ..":".. ml ..")")
end
