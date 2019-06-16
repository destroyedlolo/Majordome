-- Fermeture "My" du volet de la chambre de Joris
--
-- Ouverture du matin
-->> when=OuvertureVoletChJorisMy
--
-- Fermeture du soir
-->> when=ConsigneCoucherMy
--
-- Suivit de la température
-->> whenDone=SurveillanceChJoris

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChJoris", true)
local mode = SelShared.Get("ModeChJoris") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet de la chambre de Joris ignoré")
else	-- Vérifie en fonction de ce qui a lancer l'exécution
	if MAJORDOME_TIMER == "ConsigneCoucherMy" then
		if mode == "Absent" then
			SelLog.log('I', "Mode \"Absent\" : 'My' de fin de journée du volet de la chambre de Joris ignoré")
			return
		end
	end

	SelLog.log('A', "'My' du volet de la chambre de Joris")
	CmdVolet:Publish("My")
end
