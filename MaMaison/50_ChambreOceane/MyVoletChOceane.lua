-- Fermeture "My" du volet de la chambre d'Océane
--
-- Ouverture du matin
-->> when=OuvertureVoletChOceaneMy
--
-- Fermeture du soir
-->> when=ConsigneCoucherMy
--
-- Suivit de la température
-->> whenDone=SurveillanceChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)
local mode = SelShared.Get("ModeChOceane") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet de la chambre d'Océane ignoré")
else	-- Vérifie en fonction de ce qui a lancer l'exécution
	if MAJORDOME_TIMER == "ConsigneCoucherMy" then
		if mode == "Absent" then
			SelLog.log('I', "Mode \"Absent\" : 'My' de fin de journée du volet de la chambre d'Océane ignoré")
			return
		end
	end

	SelLog.log('A', "'My' du volet de la chambre d'Océane")
	CmdVolet:Publish("My")
end
