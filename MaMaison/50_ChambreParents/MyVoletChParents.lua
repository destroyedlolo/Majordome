-- Fermeture "My" du volet de la chambre des parents
-->> when=ConsigneCoucherMy

--
-- Récupération des objets cibles
--

local CmdVolet = MajordomeMQTTTopic.find("CmdVoletChParents", true)
local mode = SelShared.Get("ModeChParents") or "Manuel"

--
-- C'est parti
--

if mode == "Manuel" then
	SelLog.log('I', "Mode \"Manuel\" : 'My' du volet de la chambre des parents ignoré")
else	-- Vérifie en fonction de ce qui a lancer l'exécution
	if MAJORDOME_TIMER == "ConsigneCoucherMy" then
		if mode == "Absent" then
			SelLog.log('I', "Mode \"Absent\" : 'My' de fin de journée du volet de la chambre des parents ignoré")
			return
		end
	end

	SelLog.log('A', "'My' du volet de la chambre des parents")
	CmdVolet:Publish("Down")
end
