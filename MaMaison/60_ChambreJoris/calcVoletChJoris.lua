-- Calcul des automatismes du volet de la chambre de Joris
-- Paramètres qui vont lancer un recalcul
-->> listen=ModeChJoris
-->> listen=ConsigneLeverJoris

-- Vérification que tous les paramètres sont déjà présents
if not SelShared.Get("ModeChJoris")
or not SelShared.Get("ConsigneLeverJoris") then
	return
end


--
-- Récupération des timers cibles
--

local OuvertureVoletChJoris= MajordomeTimer.find("OuvertureVoletChJoris", true)
local OuvertureVoletChJorisMy = MajordomeTimer.find("OuvertureVoletChJorisMy", true)

local hl,ml = string.match(string.gsub( SelShared.Get('ConsigneLeverJoris'), '%.', ':'), "(%d+):(%d+)")


--
-- Détermination en fonction du mode
--

if SelShared.Get("ModeChJoris") == 'Vacances' then
		-- Pas d'ouverture le matin
	OuvertureVoletChJoris:Disable()
	OuvertureVoletChJorisMy:Disable()
	SelLog.log('I', "Pas d'ouverture de la chambre de Joris le matin")

elseif SelShared.Get("ModeChJoris") == 'Absent' then
		-- Ouverture directe le matin (pas de My)
	OuvertureVoletChJoris:Enable()
	OuvertureVoletChJorisMy:Disable()
	SelLog.log('I', "Ouverture directe de la chambre d'Joris le matin (".. hl ..":".. ml ..")")

else	-- Travail (le mode "Manuel est pris en charge par les scripts d'actions)
	OuvertureVoletChJoris:Enable()
	OuvertureVoletChJorisMy:Enable()
	SelLog.log('I', "My puis Ouverture de la chambre de Joris le matin (".. hl ..":".. ml ..")")
end
