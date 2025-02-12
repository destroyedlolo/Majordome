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
-->> require_topic=ModeChOceane

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChOceane", true)

--
-- C'est parti
--

if ModeChOceane:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet de la chambre d'Océane ignoré")
else	-- Vérifie en fonction de ce qui a lancer l'exécution
	if MAJORDOME_TIMER == "ConsigneCoucherMy" then
		if ModeChOceane:getVal() == "Absent" then
			SelLog.Log('I', "Mode \"Absent\" : 'My' de fin de journée du volet de la chambre d'Océane ignoré")
			return
		end
	end

	SelLog.Log('A', "'My' du volet de la chambre d'Océane")
	CmdVolet:Publish("My")
end
