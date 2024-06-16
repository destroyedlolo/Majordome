-- Fermeture "My" du volet de la chambre de Joris
--
-- Ouverture du matin
--->> when=OuvertureVoletChJorisMy
--
-- Fermeture du soir
-->> when=ConsigneCoucherMy
--
-- Suivit de la température
-->> whenDone=SurveillanceChJoris
--
-->> require_topic=ModeChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)

--
-- C'est parti
--

if ModeChJoris:getVal() == "Manuel" then
	SelLog.Log('I', "Mode \"Manuel\" : 'My' du volet de la chambre de Joris ignoré")
else	-- Vérifie en fonction de ce qui a lancer l'exécution
	if MAJORDOME_TIMER == "ConsigneCoucherMy" then
		if mode == "Absent" then
			SelLog.Log('I', "Mode \"Absent\" : 'My' de fin de journée du volet de la chambre de Joris ignoré")
			return
		end
	end

	SelLog.Log('A', "'My' du volet de la chambre de Joris")
	CmdVolet:Publish("My")
end
